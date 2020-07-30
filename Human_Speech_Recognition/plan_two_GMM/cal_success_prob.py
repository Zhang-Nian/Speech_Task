
import os
import sys


def cal_prob(file_name):
    total_num = 0
    success_num = 0
    
    f = open(file_name)
    readlines = f.readlines()

    total_num = len(readlines)

    for line in readlines:
        line = line.rstrip("\n")
        data = line.split('\t')
       
        if len(data) != 3:
            continue
       
        standard_value = int(data[1])
        eval_value = int(data[2])

        if standard_value == eval_value:
            success_num += 1

    sueecss_rate = float(success_num / total_num)
    
    print("total num is ", total_num)
    print("success num is ", success_num)
    print("sueecss_rate is ", sueecss_rate)


if __name__ == "__main__":
    file_name = sys.argv[1]
    cal_prob(file_name)

