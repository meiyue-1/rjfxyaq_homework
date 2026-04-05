#### 第一次作业 代码说明

hw1内部目录为：

```
hw1/
├── lab1/
│   ├── lab1.cpp
│   ├── test.c
│   ├── test.ll
|   ├── test
│   └── output.ll
├── lab2/
│   └── mycc.cpp
└── README.md
```

**其中lab1和lab2文件夹分别包含第一次作业的两个题目**

**lab1：**

**lab1.cpp**：实现 LLVM IR 插桩功能的 C++ 源代码，负责生成 `NewAdd` 函数并替换加法指令。

**test.c**：用于测试插桩效果的 C 语言原始代码。

**test.ll**：由 `test.c` 编译生成的**原始 LLVM IR 文件**，包含未修改的加法指令 `add`。

**test**: 最终的可执行文件

**output.ll**：经过 `lab1` 插桩后生成的**最终 IR 文件**，加法已替换为 `NewAdd` 调用。

**lab2：**

**mycc.cpp**：自定义编译器驱动程序的源代码，实现编译与自动插桩功能。
