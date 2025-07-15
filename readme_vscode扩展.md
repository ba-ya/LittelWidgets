- [生成visx扩展](#生成visx扩展)
  - [✅ 打包成 `.vsix` 的完整流程](#-打包成-vsix-的完整流程)
    - [🧰 前提条件](#-前提条件)
  - [🏗️ 创建你的主题扩展](#️-创建你的主题扩展)
    - [第一步：用 `yo` 创建项目](#第一步用-yo-创建项目)
    - [第二步：替换主题文件内容](#第二步替换主题文件内容)
    - [第三步：打包成 `.vsix`](#第三步打包成-vsix)
  - [💡 安装这个 `.vsix` 到本地 VS Code](#-安装这个-vsix-到本地-vs-code)
  - [处理不兼容问题](#处理不兼容问题)
- [xml扩展](#xml扩展)
- [目录扩展](#目录扩展)


# 生成visx扩展

------

## ✅ 打包成 `.vsix` 的完整流程

### 🧰 前提条件

确保你已经安装好以下工具：

1. **Node.js**（含 npm）
    安装地址：[https://nodejs.org](https://nodejs.org/)

2. **yo 和 generator-code**
    创建 VS Code 扩展的脚手架工具：

   ```bash
   npm install -g yo generator-code
   ```

3. **vsce**（Visual Studio Code Extension CLI）
    用于打包成 `.vsix` 文件：

   ```bash
   npm install -g vsce
   ```

------

## 🏗️ 创建你的主题扩展

### 第一步：用 `yo` 创建项目

```bash
yo code
```

接着按提示选择：

- ❓**What type of extension?** → `New Color Theme`
- ❓**Name your extension** → `qt-default-light`（或你喜欢的名字）
- ❓**Start fresh or import existing?** → `Start fresh`
- ❓**Base theme** → `Light`
- 会生成一个新文件夹（比如 `qt-default-light`），进入该目录：

```bash
cd qt-default-light
```

------

### 第二步：替换主题文件内容

打开目录里的文件：

```
qt-default-light/themes/qt-default-light-color-theme.json
```

用你之前让我帮你生成的 JSON 内容 **全部替换掉它**。

------

### 第三步：打包成 `.vsix`

在你的扩展项目根目录运行：

```bash
vsce package
```

如果一切正常，它会生成一个类似：

```
qt-default-light-0.0.1.vsix
```

🎉 这就是你的主题插件包！

------

## 💡 安装这个 `.vsix` 到本地 VS Code

在 VS Code 中按下 `Ctrl+Shift+P`，然后输入：

```
Extensions: Install from VSIX
```

选择你刚刚打好的 `qt-default-light-0.0.1.vsix` 文件。

然后再打开主题选择器：

```
Preferences: Color Theme
```

就可以看到你的主题了 🎨✨

## 处理不兼容问题

`package.json`中

```json
"engines": {
  "vscode": "^1.50.0"
}
```

# xml扩展

`xml-tool`

# 目录扩展

`Markdown All in One `

