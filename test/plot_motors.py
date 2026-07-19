#!/usr/bin/env python3
import serial
import sys
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.gridspec as gridspec

PORT     = "/dev/ttyACM0"
BAUDRATE = 115200
DT       = 0.05   # 20 Hz (delay 50ms ใน firmware)

LABELS = ["FRONT LEFT  M1", "FRONT RIGHT M2", "REAR LEFT   M3", "REAR RIGHT  M4"]
COLORS = ["#2196F3", "#E91E63", "#9C27B0", "#4CAF50"]

# ---------- collect ----------

def collect_from_serial():
    print(f"Connecting to {PORT} ...")
    try:
        ser = serial.Serial(PORT, BAUDRATE, timeout=2)
    except serial.SerialException as e:
        sys.exit(f"Cannot open {PORT}: {e}")

    ser.write(b"testall\r")
    print("Sent 'testall' — waiting for data ...\n")

    req, cur, pwm = [], [], []
    header_seen = False

    while True:
        line = ser.readline().decode("utf-8", errors="ignore").strip()
        if not line:
            continue
        print(line)

        if "req" in line.lower() and "cur" in line.lower():
            header_seen = True
            continue

        if "STOP" in line:
            break

        if header_seen:
            parts = line.split(",")
            if len(parts) == 12:
                try:
                    v = [float(p) for p in parts]
                    req.append([v[0],  v[3],  v[6],  v[9]])
                    cur.append([v[1],  v[4],  v[7],  v[10]])
                    pwm.append([v[2],  v[5],  v[8],  v[11]])
                except ValueError:
                    pass

    ser.close()
    return np.array(req), np.array(cur), np.array(pwm)


def collect_from_file(path):
    req, cur, pwm = [], [], []
    with open(path) as f:
        for line in f:
            line = line.strip()
            if not line or "req" in line.lower():
                continue
            parts = line.split(",")
            if len(parts) == 12:
                try:
                    v = [float(p) for p in parts]
                    req.append([v[0],  v[3],  v[6],  v[9]])
                    cur.append([v[1],  v[4],  v[7],  v[10]])
                    pwm.append([v[2],  v[5],  v[8],  v[11]])
                except ValueError:
                    pass
    return np.array(req), np.array(cur), np.array(pwm)

# ---------- metrics ----------

def analyze(req_col, cur_col, t):
    sp    = float(np.mean(req_col))
    upper = sp * 1.05
    lower = sp * 0.95

    # rise time 10% → 90%
    rt_s = np.where(cur_col >= 0.1 * sp)[0]
    rt_e = np.where(cur_col >= 0.9 * sp)[0]
    rise_time = (t[rt_e[0]] - t[rt_s[0]]) if len(rt_s) and len(rt_e) else float("nan")

    # peak
    peak_idx  = int(np.argmax(cur_col))
    peak_val  = float(cur_col[peak_idx])
    peak_time = float(t[peak_idx])

    # settling time ±5%
    settle_time = float("nan")
    in_band = np.where((cur_col >= lower) & (cur_col <= upper))[0]
    for idx in in_band:
        if np.all((cur_col[idx:] >= lower) & (cur_col[idx:] <= upper)):
            settle_time = float(t[idx])
            break

    overshoot = (peak_val - sp) / sp * 100 if sp else float("nan")
    steady    = float(np.mean(cur_col[-10:]))
    ss_err    = abs(sp - steady) / sp * 100 if sp else float("nan")

    return dict(sp=sp, upper=upper, lower=lower,
                rise_time=rise_time, peak_time=peak_time, peak_val=peak_val,
                settle_time=settle_time, overshoot=overshoot,
                steady=steady, ss_err=ss_err)

# ---------- plot ----------

def plot(req, cur, pwm):
    n = len(req)
    t = np.arange(n) * DT

    fig = plt.figure(figsize=(14, 7))
    fig.suptitle("Motor RPM Step Response (PI Control)", fontsize=14, fontweight="bold")
    gs  = gridspec.GridSpec(1, 4, figure=fig, hspace=0.55, wspace=0.35)

    print("\n" + "="*55)

    for i in range(4):
        m  = analyze(req[:, i], cur[:, i], t)
        ax = fig.add_subplot(gs[0, i])

        ax.plot(t, cur[:, i], color=COLORS[i], lw=2, label="actual")
        ax.plot(t, req[:, i], "r--",           lw=1.5, label="target")
        ax.axhline(m["upper"], color="green", ls="--", lw=1, alpha=0.7)
        ax.axhline(m["lower"], color="green", ls="--", lw=1, alpha=0.7)

        if not np.isnan(m["rise_time"]):
            ax.axvline(m["rise_time"] + t[np.where(cur[:, i] >= 0.1*m["sp"])[0][0]],
                       color="black",  ls="--", lw=1, label="rise")
        if not np.isnan(m["peak_time"]):
            ax.axvline(m["peak_time"],   color="magenta", ls="--", lw=1, label="peak")
        if not np.isnan(m["settle_time"]):
            ax.axvline(m["settle_time"], color="cyan",    ls="--", lw=1, label="settle")

        # metrics box — ขวาล่างของแต่ละ subplot
        box = (f"Rise:   {m['rise_time']:.2f} s\n"
               f"Peak:   {m['peak_time']:.2f} s\n"
               f"Settle: {m['settle_time']:.2f} s\n"
               f"OS:     {m['overshoot']:.1f} %\n"
               f"SS err: {m['ss_err']:.1f} %")
        ax.text(0.98, 0.02, box, transform=ax.transAxes,
                fontsize=7, va="bottom", ha="right", family="monospace",
                bbox=dict(boxstyle="round", facecolor="white", alpha=0.85, edgecolor="gray"))

        ax.set_title(LABELS[i], fontsize=9)
        ax.set_xlabel("Time (s)", fontsize=8)
        ax.set_ylabel("RPM", fontsize=8)
        ax.legend(fontsize=6, loc="lower right", bbox_to_anchor=(0.98, 0.28))
        ax.grid(True, alpha=0.3)

        # print to terminal
        print(f"\n  {LABELS[i]}")
        print(f"  Rise Time     : {m['rise_time']:.3f} s")
        print(f"  Peak Time     : {m['peak_time']:.3f} s")
        print(f"  Settling Time : {m['settle_time']:.3f} s")
        print(f"  Overshoot     : {m['overshoot']:.2f} %")
        print(f"  Peak RPM      : {m['peak_val']:.2f} RPM")
        print(f"  Steady State  : {m['steady']:.2f} RPM  (error {m['ss_err']:.2f} %)")

    print("="*55)
    plt.show()

# ---------- main ----------

if __name__ == "__main__":
    if len(sys.argv) > 1:
        # python3 plot_motors.py motor_log.txt
        req, cur, pwm = collect_from_file(sys.argv[1])
    else:
        # python3 plot_motors.py   (เชื่อม serial อัตโนมัติ)
        req, cur, pwm = collect_from_serial()

    if len(req) == 0:
        sys.exit("No data collected.")

    plot(req, cur, pwm)
