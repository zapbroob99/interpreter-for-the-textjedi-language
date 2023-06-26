# Interpreter for the TextJedi language

The TextJedi Interpreter is a command-line tool written in C that allows you to run programs written in the TextJedi programming language. TextJedi is a language designed for editing text files primarily using cutting and pasting operations.

Project Defn: Write an interpreter for the TextJedi language. Your interpreter executable should be named as TextJedi.exe and should be able to run a TextJedi program when called with an argument that contains the filename of the code to be run. (from the operating system command prompt)

## Features

The TextJedi Interpreter provides the following features:

- Variable declaration and assignment using the `new` keyword.
- Support for two data types: Strings (text) and Integers.
- Arithmetic operations: Addition (`+`) and Subtraction (`-`) on integers, concatenation (`+`) and removal (`-`) on strings.
- Predefined functions for manipulating strings: `size`, `subs`, `locate`, `insert`, `override`, `asString`, and `asText`.
- File input/output operations: Reading from a text file and writing to a text file.
- Keyboard input using prompts.
- Output display on the screen.

## Getting Started

To use the TextJedi Interpreter, follow these steps:

1. Clone the repository or download the project files.
2. Compile the source code using a C compiler.
3. Run the resulting executable with the TextJedi program file as the argument.

Example:
```
$ TextJedi myProg.tj
```

Make sure you have a TextJedi program file (e.g., `myProg.tj`) available before running the interpreter.

## Syntax and Usage

The TextJedi programming language has its own syntax and usage rules. Refer to the provided language specification document for detailed information on the language's grammar, data types, operators, functions, and commands.

A sample program in TextJedi may look like this:

/* Sample TextJedi program */

```
new text myText;
new int myInt;

myText := "Hello world";
myInt := 0;

myInt := 2 + yourInt;
myText := FirstText + SecondText;

s := "Hello" + " world";
myString := "Hello world" - "wo";

read myString from myTextFile;
write myText to yourTextFile;
input myText prompt "Enter a text: ";
output myText;

```

## Limitations

The TextJedi Interpreter has the following limitations:

- Maximum identifier size is 30 characters.
- Maximum integer size is limited to the maximum value of an unsigned int in C.
- Negative integer values are not supported.
- String constants cannot contain the double quote character.
- String constants have unlimited size, but the interpreter may have memory constraints.
- Comments must be terminated properly.
- The interpreter does not handle other file operations (e.g., file deletion or file access permissions).

## Contributing

Contributions to the TextJedi Interpreter project are welcome! If you find any issues or have suggestions for improvements, please submit an issue or a pull request on the project's GitHub repository.

## License

This project is licensed under the [MIT License](LICENSE). Feel free to use, modify, and distribute the code for personal or commercial purposes.

## Contact

For any inquiries or questions regarding the TextJedi Interpreter, please contact me at zapbroob@gmail.com.
