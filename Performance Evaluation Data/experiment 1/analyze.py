import json
import os
import numpy as np
import math
import matplotlib.pyplot as plt
import re
import copy

ROOT_DIR = "E:\\data\\"
STRUCT = {
    "COUNT": [],
    "QUEUETIME": {
        "TOTAL": [],
        "MAX": [],
        "AVG": [],
    },
    "RUNTIME": {
        "TOTAL": [],
        "MAX": [],
        "AVG": [],
    }}

LINE_TYPE = ['k-', 'b:', 'r-.', 'g--']
LABEL_TYPE = ["Not Running", "DLL", "WPM", "DLL+WPM"]
DATA_TYPE = ["MAX", "TOTAL"]
INFO_TYPE = ["QUEUETIME", "RUNTIME"]

func = {
    0: np.min,
    1: np.max,
    2: np.average,
    3: np.median,
    4: np.sum
}

func_string = {
    0: "MIN",
    1: "MAX",
    2: "AVG",
    3: "MEDIAN",
    4: "SUM"
}


def build_structure():
    tab_data = []
    for i in range(0, 4):
        tab_data.append(copy.deepcopy(STRUCT))

    global_data = []
    for i in range(0, 5):
        global_data.append(copy.deepcopy(tab_data))

    return global_data


def plot_data(global_data, n):
    for func_index in range(0, len(func)):
        plt.clf()
        plt.grid(True)
        ax = plt.subplot(111)
        for i in range(0, 4):
            data = [func[func_index](global_data[0][i]["COUNT"]),
                    func[func_index](global_data[1][i]["COUNT"]),
                    func[func_index](global_data[2][i]["COUNT"]),
                    func[func_index](global_data[3][i]["COUNT"]),
                    func[func_index](global_data[4][i]["COUNT"])]
            plt.plot([1, 2, 4, 8, 16], data, LINE_TYPE[i], marker='o', label=LABEL_TYPE[i])

        plt.xticks([1, 2, 4, 8, 16])
        plt.xlim([0, 17])

        plt.title("{0} {1} {2}".format(func_string[func_index], "COUNT", ""))
        plt.xlabel("Number of Tabs")
        plt.ylabel("Time taken (miliseconds)")

        # Shrink current axis's height by 10% on the bottom
        box = ax.get_position()
        ax.set_position([box.x0, box.y0 + box.height * 0.1, box.width, box.height * 0.9])

        # Put a legend below current axis
        lgd = ax.legend(loc='upper center', bbox_to_anchor=(0.5, -0.1), ncol=2)

        plt.savefig("E:\\results\\{1}_{2}_{3}_{0}.png".format(n, func_string[func_index], "COUNT", ""),
                    bbox_extra_artists=(lgd,), bbox_inches='tight')

    for func_index in range(0, len(func)):
        for info in INFO_TYPE:
            plt.clf()
            plt.grid(True)
            ax = plt.subplot(111)
            for i in range(0, 4):
                data = [func[func_index](global_data[0][i][info]["TOTAL"] / np.sum(global_data[0][i]["COUNT"])),
                        func[func_index](global_data[1][i][info]["TOTAL"] / np.sum(global_data[1][i]["COUNT"])),
                        func[func_index](global_data[2][i][info]["TOTAL"] / np.sum(global_data[2][i]["COUNT"])),
                        func[func_index](global_data[3][i][info]["TOTAL"] / np.sum(global_data[3][i]["COUNT"])),
                        func[func_index](global_data[4][i][info]["TOTAL"] / np.sum(global_data[4][i]["COUNT"]))]
                plt.plot([1, 2, 4, 8, 16], data, LINE_TYPE[i], marker='o', label=LABEL_TYPE[i])

            plt.xticks([1, 2, 4, 8, 16])
            plt.xlim([0, 17])

            plt.title("{0} {1} {2}".format(func_string[func_index], info, "AVG"))
            plt.xlabel("Number of Tabs")
            plt.ylabel("Time taken (miliseconds)")

            # Shrink current axis's height by 10% on the bottom
            box = ax.get_position()
            ax.set_position([box.x0, box.y0 + box.height * 0.1, box.width, box.height * 0.9])

            # Put a legend below current axis
            lgd = ax.legend(loc='upper center', bbox_to_anchor=(0.5, -0.1), ncol=2)

            plt.savefig("E:\\results\\{1}_{2}_{3}_{0}.png".format(n, func_string[func_index], info, "AVG"),
                        bbox_extra_artists=(lgd,), bbox_inches='tight')

    for func_index in range(0, len(func)):
        for info in INFO_TYPE:
            for dtype in DATA_TYPE:
                plt.clf()
                plt.grid(True)
                ax = plt.subplot(111)
                for i in range(0, 4):
                    data = [func[func_index](global_data[0][i][info][dtype]),
                            func[func_index](global_data[1][i][info][dtype]),
                            func[func_index](global_data[2][i][info][dtype]),
                            func[func_index](global_data[3][i][info][dtype]),
                            func[func_index](global_data[4][i][info][dtype])]
                    plt.plot([1, 2, 4, 8, 16], data, LINE_TYPE[i], marker='o', label=LABEL_TYPE[i])

                plt.xticks([1, 2, 4, 8, 16])
                plt.xlim([0, 17])

                plt.title("{0} {1} {2}".format(func_string[func_index], info, dtype))
                plt.xlabel("Number of Tabs")
                plt.ylabel("Time taken (miliseconds)")

                # Shrink current axis's height by 10% on the bottom
                box = ax.get_position()
                ax.set_position([box.x0, box.y0 + box.height * 0.1, box.width, box.height * 0.9])

                # Put a legend below current axis
                lgd = ax.legend(loc='upper center', bbox_to_anchor=(0.5, -0.1), ncol=2)

                plt.savefig("E:\\results\\{1}_{2}_{3}_{0}.png".format(n, func_string[func_index], info, dtype),
                            bbox_extra_artists=(lgd,), bbox_inches='tight')


def read_data(n):
    global_data = build_structure()

    for filename in os.listdir(ROOT_DIR):
        m = re.match("(\d+)-(\d+)", filename)
        num_tabs = int(math.log(int(m.group(1)), 2))
        num_mode = int(m.group(2)) - 1

        with open(ROOT_DIR + "\\" + filename, 'r') as infile:
            raw_data = json.load(infile)

        for i in range(0, n):
            snapshot = raw_data["snapshots"][i]

            for data in snapshot["data"]:
                for elem in data["list"]:
                    global_data[num_tabs][num_mode]["COUNT"].append(elem["death_data"]["count"])
                    global_data[num_tabs][num_mode]["QUEUETIME"]["TOTAL"].append(elem["death_data"]["queue_ms"])
                    global_data[num_tabs][num_mode]["QUEUETIME"]["MAX"].append(elem["death_data"]["queue_ms_max"])
                    global_data[num_tabs][num_mode]["QUEUETIME"]["AVG"].append(elem["death_data"]["queue_ms_sample"])
                    global_data[num_tabs][num_mode]["RUNTIME"]["TOTAL"].append(elem["death_data"]["run_ms"])
                    global_data[num_tabs][num_mode]["RUNTIME"]["MAX"].append(elem["death_data"]["run_ms_max"])
                    global_data[num_tabs][num_mode]["RUNTIME"]["AVG"].append(elem["death_data"]["run_ms_sample"])

    return global_data


if __name__ == '__main__':
    for i in [1, 2, 4, 8, 16]:
        plot_data(read_data(int(math.log(i, 2)) + 1), i)
