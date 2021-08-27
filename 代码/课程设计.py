def Answer():
    """计算出每一题的答案"""
    fi = open("answer.txt", "r").readlines()
    stu_ans_ls = list(fi)
    ans = []
    for i in range(35):
        counts = {}
        for word in stu_ans_ls[i]:
            counts[word] = counts.get(word, 0) + 1  # 计算每一题四个选项分别出现的次数
        # 将四个选项中出现次数最多的选项及出现频次赋值给max_ans
        max_ans = max(zip(counts.values(), counts.keys()))
        # 按顺序存储每一题的答案和它出现的频次，存储在列表中
        ans.append([max_ans[1], max_ans[0]])
    return ans


def Output_Answer():
    """输出参考答案"""
    ans = Answer()
    print("{:<5}{:<5}{:>1}".format('题号', '答案', '频次'))
    for i in range(35):
        print("{:<5}{:^5}{:>5}".format(i + 1, ans[i][0], ans[i][1]))


def Scoring():
    stu_ls = []
    scores = []
    ans = Answer()
    """计算学生分数"""
    fi_score = open("sample.txt", "r")

    for line in fi_score:
        # print(type(line))
        stu_ls.append(line.strip("\n").split())

    for i in range(91):
        score = 0
        for j in range(15):
            if stu_ls[i][2][j] == ans[j][0]:
                score += 2
        for j in range(15, 25):
            if stu_ls[i][2][j] == ans[j][0]:
                score += 3
        for j in range(25, 35):
            if stu_ls[i][2][j] == ans[j][0]:
                score += 4
        scores.append([stu_ls[i][1], str(score)])  # scores:['赵光', '63'], ['罗志', '84']
        stu_ls[i].append(str(score))  # 将得分与人名对应
        # scores.sort(key=lambda x: eval(x[1]), reverse=True)
    return stu_ls, scores


def Sort():
    stu_ls, scores = Scoring()
    ls_tmp = sorted(stu_ls, key=lambda x: eval(x[3]), reverse=True)
    # print(stu_ls)
    print("{:<5}{:<6}{:<7}{:<5}".format('排名', '学号', '姓名', '成绩'))
    for i in range(91):
        print("{:<2}{:^11}{:<6}{:^5}".format(i + 1, ls_tmp[i][0], ls_tmp[i][1], ls_tmp[i][3]))


def Find():
    """查询学生成绩"""
    stu_ls, scores = Scoring()
    name = input("请输入要查询的名字：")
    while name:
        for i in range(91):
            if name in stu_ls[i][1]:
                print(stu_ls[i][1], stu_ls[i][3])
                break
        else:
            print("您要查询的名字不存在！")

        name = input("请输入要查询的名字：")


def Result():
    """输出成绩分析"""
    stu_ls, scores = Scoring()
    pass_num = sum_scores = 0
    lv1 = lv2 = lv3 = lv4 = lv5 = 0
    for i in range(91):
        sum_scores += eval(stu_ls[i][3])

        # 按分值计算每个学生的分数
        if eval(stu_ls[i][3]) < 60:
            lv1 += 1
        elif 70 > eval(stu_ls[i][3]) >= 60:
            lv2 += 1
        elif 80 > eval(stu_ls[i][3]) >= 70:
            lv3 += 1
        elif 90 > eval(stu_ls[i][3]) >= 80:
            lv4 += 1
        else:
            lv5 += 1
        # 所有及格的人数
        pass_num = sum({lv2, lv3, lv4, lv5})

    ls_tmp1 = sorted(stu_ls, key=lambda x: eval(x[3]), reverse=True)
    # print(len(scores))
    print("最高分:{:<5}{:<5}\n最底分:{:<5}{:<5}\n"
          .format(ls_tmp1[0][1], ls_tmp1[0][3], ls_tmp1[90][1], ls_tmp1[90][3]))

    print("不及格的有{:>10}人\n60~69的有{:>10}人\n70~79的有{:>10}人\n80~89的有{:>10}人\n90以上的有{:>10}人\n"
          .format(lv1, lv2, lv3, lv4, lv5))
    print("平均分：{:<10.2f}及格率：{:>5.2f}".format(sum_scores / 91, pass_num / 91, ))


def main():
    print("请选择功能")
    print("1、输出答案\n"
          "2、排名\n"
          "3、查找\n"
          "4、考试分析")
    n = input("请输入：")
    try:
        while n:
            if eval(n) == 1:
                Output_Answer()
                # break
            elif eval(n) == 2:
                Sort()
                # break
            elif eval(n) == 3:
                Find()
                # break
            elif eval(n) == 4:
                Result()
                # break
            else:
                print("输入错误\n")
                # break

            print("\n请选择功能：")
            print("1、输出答案\n"
                  "2、排名\n"
                  "3、查找\n"
                  "4、考试分析")
            n = input("\n请选择功能：")
    except NameError:
        print("输入错误，请输入选择的功能序号\n")


if __name__ == '__main__':
    main()
