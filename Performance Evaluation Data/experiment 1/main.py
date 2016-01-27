import time
import subprocess
import json
from shutil import copyfile

TABS = [1, 2, 4, 8, 16]
MODE = [1, 2, 3, 4]
ITERATION = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
data = {
    "tabs": 0,
    "mode": 0,
    "iteration": 0
}
lastMode = 0


def loadconfig():
    global data, lastMode
    with open('D:\\data\\data.json', 'r') as infile:
        data = json.load(infile)

    lastMode = data["mode"]
    print "Config is: {0}-{1}-{2}".format(TABS[data["tabs"]], MODE[data["mode"]], ITERATION[data["iteration"]])


def writeconfig():
    global data
    data["iteration"] = (data["iteration"] + 1) % 10
    if data["iteration"] == 0:
        data["tabs"] = (data["tabs"] + 1) % 5

        if data["tabs"] == 0:
            data["mode"] = (data["mode"] + 1) % 4

    with open('D:\\data\\data.json', 'w') as outfile:
        json.dump(data, outfile)


def copy():
    global data
    copyfile('C:\\Users\\user\\Downloads\\profile.json',
             "D:\\data\\{0}-{1}-{2}.json".format(TABS[data["tabs"]], MODE[data["mode"]], ITERATION[data["iteration"]]))


def main():
    global data
    print "Running benchmarks"
    path = "--profile-directory=\"{0} Tabs\"".format(TABS[data["tabs"]])
    command = ["D:\workspace\chromium\src\out\Release\chrome.exe", "--args", path]
    print " ".join(command)
    subprocess.Popen(" ".join(command))
    print "Sleeping 17 minutes"
    time.sleep(60 * 17)
    print "Reboot"
    copy()
    writeconfig()

    if data["mode"] != lastMode:
        exit(0)
    subprocess.call("shutdown -r -t 0")


if __name__ == '__main__':
    loadconfig()
    main()
