# Lab 1：Data Lab 实验指南

Due: 9 Oct, 23:59:59

本实验通过受限的整数运算和 IEEE 754 单精度浮点数位级操作，练习二进制补码、掩码、移位、溢出和舍入。

你只需要修改 `bits.c` 中 P1–P19 的函数体。不要修改函数名、参数、返回类型、测试程序或评分配置。全部通过时，`btest` 最后一行应为：

```text
Total points: 110/110
```

我们将根据你的 Github 仓库中最终的自动测试结果评分。请把你的修改提交到 Github 仓库，并把仓库链接提交至 E-Learning 平台。

## 部署实验环境

### 环境要求

在支持 32 位编译的 x86-64 Linux 环境中完成实验可以获得完整评测体验，例如课程服务器、Ubuntu 虚拟机或 WSL。仓库中的 `dlc` 是 Linux ELF 可执行文件，不能直接在 macOS 或 Windows 上运行。

若在不支持评测的系统如 M 系列芯片 MacOS 完成实验，也可以通过 Github Workflow 线上查看评测结果。完成提交和推送后，在仓库的 Actions 页面点入 Workflow 详情后，可以通过 Annotations 查看总分，也可以点击 run-autograding-tests 进入每个测试的详细流程检查，查看 Autograding Reporter 项（方法不稳定，可能需要多次刷新）或在右侧设置项中下载日志查看测试具体情况。

**请注意不要修改 .github 文件夹内预设的自动化测试内容。**

### 32 位编译 Linux 环境配置

执行：

```shell
sudo apt-get update
sudo apt-get install -y gcc make gcc-multilib libc6-dev-i386 python3
```

如果编译时提示缺少 `bits/libc-header-start.h`、`-lgcc` 或其他 32 位库，通常是 `gcc-multilib` 或 `libc6-dev-i386` 未安装完整。

### 确认实验文件能正常构建

键入 `ls`，你应当看到如下文件：

```text
Driverhdrs.pm  Driverlib.pm  Makefile  README.md  bits.c  bits.h  btest.c
btest.h  check_ops.py  decl.c  dlc  driver.pl  fshow.c  ishow.c  test.sh  tests.c
```

在终端中依次执行下述指令，以生成可执行文件并执行：

```shell
make clean
make all
./btest
```

`make all` 会生成 `btest`、`ishow`、`fshow` 三个可执行文件。如果过程顺利，`./btest` 的最后一行会输出 `Total points: 0/110` 。

如果遇到 `./check_ops.py: Permission denied` ，说明当前文件没有执行权限，执行：

```shell
chmod +x check_ops.py dlc
```

注意 `btest` 不会在源文件修改后自动重编译。每次改完 `bits.c` 都要重新执行 `make clean && make all` ，否则测试的可能仍是旧代码。

## 主要文件

| 文件 | 用途 |
|---|---|
| `README.md` | 实验说明，包含每道题的完整规则 |
| `bits.c` | 唯一需要填写的代码文件；文件开头包含完整编码规则 |
| `bits.h` | 函数声明，不能修改 |
| `check_ops.py` | 当前题目的规则与操作数检查入口 |
| `dlc` | `check_ops.py` 内部调用的 DataLab 规则检查器 |
| `decl.c`、`tests.c`、`btest.c` | 题目参数范围、参考行为与正确性测试 |
| `Makefile` | 构建 `btest`、`ishow` 和 `fshow` |
| `test.sh` | 一次执行构建、规则检查和完整测试 |
| `ishow`、`fshow` | 编译后生成的整数和浮点位表示辅助工具 |

开始前建议依次阅读：

1. 本文档；
2. `bits.c` 文件开头的编码规则；
3. 每个函数上方的题意、合法运算符、最大操作数和输入范围。

## 编码规则

### 32 位整数机器模型

整数题采用 32 位机器模型：

- `int` 是 32 位二进制补码；
- 对有符号整数执行 `>>` 时采用算术右移，高位补符号位；
- 加法和左移按 32 位位向量解释，包括左移进入符号位的情况；
- 超出 32 位的高位被丢弃；
- 移位量必须在 0–31 内。

构建脚本使用 `-m32 -fwrapv`，提交应以课程 Linux 环境中的检查结果为准。生成最高位掩码、把字节移入最高字节、计算加法的低 32 位等写法都依赖上述模型。

### 整数题

除 P2 有更严格限制外，整数题只允许：

- 常量 `0` 到 `255`，含十六进制形式 `0x00`–`0xFF`；
- 函数参数和 `int` 类型局部变量；
- 一元运算符 `!`、`~`；
- 二元运算符 `&`、`^`、`|`、`+`、`<<`、`>>`。

整数题禁止：

- `if`、`for`、`while`、`do`、`switch`；
- `&&`、`||`、`-`、`*`、`/`、`%`、三目运算符；
- 比较运算符，如 `<`、`<=`、`>`、`>=`、`==`、`!=`；
- 宏、额外函数、函数调用、类型转换；
- `int` 以外的数据类型，包括 `unsigned`；
- 数组、结构体、联合体；
- 超过该题最大操作数限制的实现。

赋值本身不计入操作数，但赋值右侧使用的运算符会计数。负数常量也会涉及禁用的负号，应使用允许的运算构造所需位模式。

P2 `bitXor` 只能使用 `~` 和 `&`，不能使用其他通常允许的整数运算符。

### 浮点题

浮点题操作的是单精度浮点数的 32 位编码。参数和返回值使用 `int` 或 `unsigned`，不能直接使用 `float`。

浮点题允许：

- `int`、`unsigned` 类型和任意整数常量；
- 整数算术、位运算、逻辑运算和比较；
- `if`、`while`、`for` 等控制语句。

浮点题仍然禁止：

- 浮点类型、浮点常量和浮点运算；
- 类型转换；
- 宏、额外函数和函数调用；
- `int`、`unsigned` 以外的数据类型；
- 数组、结构体和联合体。

## 逐题规则表

整数题表中的“通用整数运算”指 `! ~ & ^ | + << >>`。

| 编号 | 函数 | 输入约束 | 合法运算符/语法 | 最大操作数 | 分值 |
|---:|---|---|---|---:|---:|
| P1 | `signMask()` | 无参数 | 通用整数运算 | 2 | 1 |
| P2 | `bitXor(x,y)` | 任意 `int` | 仅 `~ &` | 8 | 2 |
| P3 | `negativePart(x)` | 任意 `int` | 通用整数运算 | 6 | 3 |
| P4 | `copyByteWithin(x,src,dst)` | `src,dst` 为 0–3 | 通用整数运算 | 12 | 4 |
| P5 | `logicalShift(x,n)` | `0<=n<=31` | 通用整数运算 | 20 | 4 |
| P6 | `swapNibblePairs(x)` | 任意 `int` | 通用整数运算 | 18 | 4 |
| P7 | `secondLowestZeroBit(x)` | 任意 `int` | 通用整数运算 | 8 | 4 |
| P8 | `oddParity(x)` | 任意 `int` | 通用整数运算 | 56 | 5 |
| P9 | `rotateRightBits(x,n)` | `0<=n<=INT_MAX`，实际位数按 32 取模 | 通用整数运算 | 16 | 5 |
| P10 | `roundEvenPow2(x,n)` | `0<=x<=0x3fffffff`，`1<=n<=16` | 通用整数运算 | 24 | 5 |
| P11 | `midpointTowardFirst(x,y)` | 任意 `int` | 通用整数运算 | 32 | 5 |
| P12 | `isBetweenEitherOrder(x,a,b)` | 任意 `int` | 通用整数运算 | 48 | 7 |
| P13 | `mul5Sat(x)` | 任意 `int` | 通用整数运算 | 30 | 7 |
| P14 | `classifyAdd3(x,y,z)` | 任意 `int` | 通用整数运算 | 52 | 7 |
| P15 | `floatScaleThreeHalves(uf)` | 任意单精度位模式 | 浮点题整数规则与控制流 | 60 | 7 |
| P16 | `floatRoundEven(uf)` | 任意单精度位模式 | 浮点题整数规则与控制流 | 65 | 10 |
| P17 | `float_i2f(x)` | 任意 `int` | 浮点题整数规则与控制流 | 40 | 10 |
| P18 | `bitCount(x)` | 任意 `int` | 通用整数运算 | 40 | 10 |
| P19 | `bitReverse(x)` | 任意 `int` | 通用整数运算 | 34 | 10 |

具体输出语义、特殊值行为和示例请阅读 `bits.c` 中对应函数的注释。

## 推荐完成流程

### 完成题目

动手前先确认：

1. 输入、输出和参数范围；
2. 可用运算符和最大操作数；
3. 是否涉及 `INT_MIN`、`INT_MAX`、符号变化或溢出；
4. 移位量是否可能为 0 或 31；
5. 浮点题是否涉及非规格化数、NaN、无穷大或舍入中点。

### 编译

每次修改 `bits.c` 后重新编译：

```bash
make clean
make all
```

`btest` 不会在源文件修改后自动重编译。若忘记重新编译，测试的可能仍是旧代码。

### 检查运算符和操作数

```bash
./check_ops.py bits.c
```

输出会列出每道题的“当前操作数/最大操作数”。最后出现以下内容才表示规则检查通过：

```text
All 19 functions passed operator checks.
```

不要直接使用 `./dlc bits.c` 检查整份文件。仓库自带的 `dlc` 内置了旧函数名，`check_ops.py` 会为新题逐题选择同规则的代理函数名，再调用 `dlc` 检查实际函数体。代理过程不会放宽合法运算符，只解决旧二进制不认识新函数名的问题。

规则检查通过只说明代码写法合法，不表示结果正确。

### 测试单题

```bash
./btest -f 函数名
```

例如：

```bash
./btest -f roundEvenPow2
```

可用 `-1`、`-2`、`-3` 指定前三个参数：

```bash
./btest -f roundEvenPow2 -1 5 -2 1
./btest -f copyByteWithin -1 0x11223344 -2 0 -3 2
```

失败时会显示输入、实际结果和期望结果。不要只针对一个失败样例打补丁，应继续检查同类边界。

### 完整测试

```bash
./btest
```

每道题的 `Errors` 都应为 0，满分为 `110/110`。

也可以一次执行全部流程：

```bash
./test.sh
```

`test.sh` 只有在构建成功、规则检查通过并取得 `110/110` 时才返回状态码 0。在尚未完成全部题目时返回非零是正常现象，调试时应优先使用单题检查。

## 调试工具

### 查看整数表示

```bash
./ishow 27
./ishow 0x80000000
```

`ishow` 会显示十六进制、有符号和无符号解释。

### 查看浮点表示

```bash
./fshow 0x3f800000
./fshow 0x80000000
```

`fshow` 会拆解符号位、阶码和尾数。

### 优先测试的边界

整数题通常应关注：

- `0`、`1`、`-1`、`INT_MIN`、`INT_MAX`；
- 同号与异号输入；
- 移位量 0、1、31，以及循环移位的 32 倍数；
- 字节和半字节边界；
- 刚好不溢出与刚好溢出的值；
- 舍入余数小于、等于、大于半程的情况。

浮点题通常应关注：

- `+0`、`-0`；
- 最小和最大非规格化数；
- 最小规格化数及其相邻位模式；
- `0.25`、`0.5`、`1.5`、`2.5` 等舍入边界及其负数；
- 最大有限数、正负无穷；
- 不同符号和 payload 的 NaN。

## 常见问题

### `./check_ops.py: Permission denied`

```bash
chmod +x check_ops.py dlc
```

### `./dlc: cannot execute binary file`

当前环境不是兼容的 x86-64 Linux，请切换到课程服务器、Linux 虚拟机或合适的 WSL 环境。

### 编译提示缺少 32 位库

```bash
sudo apt-get install -y gcc-multilib libc6-dev-i386
make clean
make all
```

### 规则检查通过但 `btest` 失败

检查器只验证语法、运算符和操作数数量；`btest` 才验证结果。根据失败输入判断问题属于符号、溢出、移位、舍入还是浮点特殊值处理。

### `btest` 通过但规则检查失败

算法结果可能正确，但使用了禁用语法、常量或过多操作数，仍不能得分。按 `check_ops.py` 给出的函数定位问题。

### `test.sh` 在未完成实验时失败

这是预期行为，因为脚本要求满分。使用 `./btest -f 函数名` 调试单题。

## 提交前自查

- 只修改了 `bits.c` 中允许填写的函数体；
- 没有改变函数签名、测试文件或评分配置；
- `make clean && make all` 成功；
- `./check_ops.py bits.c` 的 19 题均通过；
- `./btest` 得到 `110/110`；
- 没有提交 `btest`、`ishow`、`fshow`、`*.o` 等编译产物；
- 最终提交已经推送，有自动测试结果。
