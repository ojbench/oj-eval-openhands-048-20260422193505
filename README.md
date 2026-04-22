# Problem 048 - Tetris

**ACMOJ Problem ID**: 2455

## Table of Contents

- [Problem 048 - Tetris](#problem-048-tetris)
  - [Table of Contents](#table-of-contents)
  - [Introduction](#introduction)
    - [Background](#background)
  - [Assignment Description](#assignment-description)
    - [Grade Composition](#grade-composition)
  - [Assignment Requirements](#assignment-requirements)
    - [Input Format](#input-format)
    - [Output Format](#output-format)
    - [Samples](#samples)
    - [Data Constraints](#data-constraints)
  - [Per-Testcase Resource Limits](#per-testcase-resource-limits)
  - [Test Data](#test-data)
  - [Submission Requirements](#submission-requirements)
    - [Submission Format](#submission-format)
    - [Submission Guidelines](#submission-guidelines)
    - [Evaluation Notes](#evaluation-notes)
    - [Academic Integrity](#academic-integrity)

## Introduction

### Background

Implement Tetris game: 7 tetrominoes, rotation, movement, line clearing, scoring, game-over detection.

## Assignment Description

### Grade Composition

| Grading Component | Percentage |
| :--: | :--: |
| Pass **2455. Tetris** | 80% |
| Code Review | 20% |

Here are several points that need clarification:

- In the Code Review, we will **strictly examine your code style and repository organization structure, etc.**

- This assignment provides some sample data for testing, stored in the `/workspace/data/048/data_test/` directory. Note that these are not the test cases on the Online Judge. Passing all local test cases does not guarantee that you will pass the OJ tests.

- Besides the provided sample data, we also encourage you to design your own test data based on your program logic to assist debugging.

## Assignment Requirements

### Problem Description

**[下发文件戳我捏](https://jbox.sjtu.edu.cn/l/d1FTm7) / [attachment]T3.zip[/attachment]**

> updated by sword: 左侧链接已失效，下载下发文件请用右侧附件或者最下方的附件，感谢 2023 级助教 @baihua 提供的题面和数据

> 有研究者发现玩俄罗斯方块游戏（Tetris, Тетрис）有助于防止创伤后应激障碍（PTSD）的发生，可能是这个游戏能够对大脑储存视觉记忆的功能产生干扰，从而保护病人免受创伤后应激反应的影响。

baihua 最近在物理课、数学课、编译器、系统课、助教工作、课堂pre、学子讲坛等多重压力下终于是熬过了地狱一般的十月，但是也很不幸患上了 PTSD。为了治疗 PTSD，baihua 希望你编写一个简易版俄罗斯方块小游戏供他玩耍。

> 如果你不了解什么是俄罗斯方块，请点击链接以了解更多：[不会吧不会吧，不会有人连俄罗斯方块都不知道吧](https://en.wikipedia.org/wiki/Tetris)

baihua 希望你写的简易版俄罗斯方块要“尊重原著”，也就是说，你写的俄罗斯方块的大致规则如下：

> 由 7 种四格骨牌构成，全部都由四个方块组成。开始时，一个随机的方块会从区域上方开始缓慢继续落下。落下期间，玩家可以以 $90^{\circ}$ 为单位旋转方块，以格子为单位左右移动方块，或让方块加速落下。当方块下落到区域最下方或着落到其他方块上无法再向下移动时，就会“锁定”在该处，然后一个新的随机的方块会出现在区域上方开始落下。当区域中某一横行的格子全部由方块填满时，则该行会被消除并成为玩家的得分。同时消除的行数越多，每行平均得分越多。

为了有更好的游戏体验，也为了减轻你的工作量，baihua 对简易版俄罗斯方块的一些细节机制规定如下：

- 游戏区域（Feild）：高 $20$ 格，宽 $10$ 格

- 骨牌（Tetromino）：
  共有 7 种骨牌，每种骨牌的形状不同，因此其旋转中心也不同。同时，由于骨牌不一定中心对称或者其对称中心不一定在骨牌的某个方块中心，所以骨牌的每一次旋转过后其旋转中心可能也会改变，旋转 $360^{\circ}$（即旋转 4 次） 为骨牌旋转中心的变化周期。为了统一，下文的“状态”包括骨牌的形状、方向和旋转中心。7 种骨牌的形状均对应着一个大写英文字母，分别为 `I`、`T`、`O`、`S`、`Z`、`J`、`L`。
  
  下图按照上述顺序依次展示了 7 种骨牌的形状、初始状态以及旋转过程中旋转中心的变化，其中每行分别展示了一种骨牌，最左端的为初始状态，小黑点表示当前状态下骨牌下一次旋转的中心，旋转四次后骨牌回到初始状态。
<img src="https://acm.sjtu.edu.cn/OnlineJudge/oj-images/fbd5c2f6-57bc-418c-960f-51e57205f0c7" style="width: 100%">

不难发现，骨牌的旋转不仅使其方向改变，同时也使其左上角方格（即能覆盖骨牌的最小矩形的左上角）的位置发生了改变，比如 `T` 骨牌从初始状态旋转到下一状态时，其左上角方格在 feild 中的位置向上移动了一格。

不过要让你分析出 7 种骨牌各自 4 种状态左上角方格的坐标变化的工作量太大了，所以 baihua 已经帮你在下发的模板中打好了表，你**只需要在充分理解上述处理旋转的方法后直接使用模板即可。**

- 操作（Operation）：
  - `w`：顺时针旋转骨牌$90^{\circ}$
  - `a`：将骨牌向左移动一格
  - `d`：将骨牌向右移动一格
  - `s`：将骨牌向下移动一格（即加速下落一格）
  - `e`：不做任何操作（此操作一定会操作成功）
  - `space`：使骨牌直接下落到底（此操作一定会操作成功）
  - `esc`：直接退出游戏

- 游戏运行流程：
  1. 输入骨牌生成种子 `seed`，随即启动游戏；
  2. 启动游戏后立即生成一个骨牌并输出生成的骨牌的种类（一个大写英文字母）并**换行**；
  3. 之后每个单位时间内，游戏会依次执行以下操作：
	- 打印当前区域方块分布图；
    - 要求玩家输入一个操作（一个字符串）。
      - 若输入的操作不合法（即不是上述操作中的任何一种）则要求玩家重新输入操作直到输入合法操作后再执行该操作，该过程不会占用游戏时间。
      - 当输入的操作合法时，若该操作为 `esc` 则输出 `See you!\n` ，结算分数并输出 `Your score: <score>\n` 后直接关闭游戏。
	  - 若为其他操作则执行该操作，若成功执行则输出 `Successful operation!\n`，若失败则什么操作都不做并输出 `Failed operation!\n`，无论操作是否成功执行该回合均不能再继续其他操作;
    - 骨牌下降一格，若能成功下落则进入下一个单位时间，若不能下落则“锁定”骨牌（和原版游戏一样，“锁定”的方块仅会与它所在的那一行一起下落，与它所在行的其他方块的相对位置不会再改变）
    - “锁定”骨牌后消除被方块填满的行并计算分数（仅计算即可，无需输出分数），检查结束条件(a)（后文有详细说明）是否满足，若满足则先打印此时的方块分布图，随后按如下格式输出结束提示语 `Game Over!\n`，随后结算分数，输出 `Your score: <score>\n`。
      
      若不满足则生成一个新的骨牌并输出一个大写英文字母表示骨牌的种类（需要换行），这之后需要检查是否满足结束条件(b)，如果满足则按如下格式输出结束提示语 `Game Over!\n`，随后结算分数，输出 `Your score: <score>\n`，但无需打印方块分布图。如果不满足则继续下一个单位时间的游戏。

- 其他机制：
  - 关于骨牌和操作：
    - 每次操作占用一个单位时间，每个单位时间末骨牌会下降一格
    - 骨牌落到不能再下落时才会“锁定”，结合上一条，即若骨牌已经贴着下方的方块堆，玩家仍然可以执行一次**可行的**操作
    - 旋转操作仅考虑初状态和末状态是否与方块堆或边界碰撞，不考虑旋转的过程中的碰撞情况
  - 关于生成骨牌：
    - 刚生成的骨牌的状态均为上文提到的“初始状态”
    - 骨牌的生成位置在区域最顶部的中间位置，具体生成位置会在下发文件中给出
	- 采用 XorShift 算法随机生成新骨牌，具体代码已经在下发文件中给出
  - 关于计分规则：
    - 一次消除一行共得 100 分
    - 一次消除两行共得 300 分
    - 一次消除三行共得 600 分
    - 一次消除四行共得 1000 分
  - 关于游戏结束条件：游戏结束需满足以下两个条件中任意一个
    - 条件(a)：方块堆触及区域顶部，即所有方块中最高的方块位于区域最顶部的一行
	- 条件(b)：生成新骨牌时检测到与方块堆的冲突，即生成新骨牌所占用的格子中已经存在方块
  - 关于打印区域方块分布图的格式：`1` 代表存在方块，`0` 代表不存在方块，你每次需要打印一个高 $20$ 格、宽 $10$ 格的矩阵，数字之间用空格隔开，你的矩阵中既需要标识出底部已“锁定”的方块所占的格子，也需要标识出目前正在坠落的骨牌所占的格子。

### Input Format

第一行输入一个 `int` 范围内的整数 `seed`；

之后若干行每行输入一个字符串代表操作。

### Output Format

见题面及下发样例。

### Samples

#### Sample 1

### Data Constraints

<table border="1">
    <tr>
        <th>testcase</th>
        <th>content</th>
		<th>points</th>
    </tr>
    <tr>
        <td>1</td>
		<td>测试 <code>a</code>，<code>d</code>，<code>s</code>，<code>e</code>，<code>esc</code> 操作的正确性</td>
		<td>10</td>
    </tr>
    <tr>
        <td>2</td>
        <td>测试 <code>w</code>，<code>a</code>，<code>d</code>，<code>s</code>，<code>e</code>，<code>esc</code> 操作的正确性</td>
		<td>15</td>
    </tr>
	<tr>
	    <td>3</td>
        <td>测试 <code>w</code>，<code>a</code>，<code>d</code>，<code>s</code>，<code>e</code>，<code>esc</code> 操作及计分的正确性</td>
		<td>20</td>
	</tr>
	<tr>
		<td>4</td>
		<td>测试结束游戏机制的正确性</td>
		<td>15</td>
	</tr>
	<tr>
  	    <td>5</td>
		<td>测试游戏结束机制的正确性</td>
		<td>15</td>
	</tr>
	<tr>
		<td>6</td>
		<td>综合测试游戏的正确性</td>
		<td>25</td>
	</tr>
</table>

## Per-Testcase Resource Limits

- **Time Limit (per test case)**: 1000 ms
- **Memory Limit (per test case)**: 256 MiB
- **Disk Usage**: No disk usage is permitted.

## Test Data

The test data for this problem is located at `/workspace/data/048/data_test/`.

Attachments and starter files for this problem are located at `/workspace/data/048/`.

## Submission Requirements

### Submission Format

Submit a single C++ source file to the Online Judge. The OJ provides its own `main.cpp` which `#include "src.hpp"`. Your submission will be written to `src.hpp` and compiled together with the OJ's driver code.

### Submission Guidelines

- The submitted code must be able to compile successfully through the above compilation process
- The program needs to be able to read data from standard input and write results to standard output
- Please ensure the code runs correctly within the given time and space limits
- **You must use C++ or C language** to implement this assignment

### Evaluation Notes

- The evaluation system will test your program using the provided test data
- The program output must exactly match the expected output (including format)
- Exceeding time or memory limits will be judged as the corresponding error type
- Please pay attention to the overall time performance of your code and the time complexity of each part of your algorithm.

### Academic Integrity

If any violations are found during evaluation or code review (including but not limited to using unconventional methods to pass test cases), your final score may be significantly reduced or become **0 points**.
