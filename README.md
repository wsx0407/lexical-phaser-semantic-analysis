# 
任务1：创建一个词法分析程序，该程序支持分析常规语法。
您必须使用DFA（确定性有限自动机）或NFA（不确定性有限自动机）来实现此程序。 程序有两个输入：1）一个文本文档，包括一组3º语法（正规文法）的产生式；2）一个源代码文本文档，包含一组需要识别的字符串。 程序的输出是一个token（令牌）表，该表由5种token组成：关键词，标识符，常量，限定符和运算符。
词法分析程序的推荐处理逻辑：根据用户输入的正规文法，生成NFA，再确定化生成DFA，根据DFA编写识别token的程序，从头到尾从左至右识别用户输入的源代码，生成token列表（三元组：所在行号，类别，token 内容）
要求：词法分析程序可以 准确识别 科学计数法形式的常量（如0.314E+1），复数常量（如10+12i），可检查整数产量的合法性，标识符的合法性（首字符不能为数字等），尽量符合真实常用高级语言（如C++、Java或python）要求的规则

Task 1: Create a lexical analysis program,which supports analyzing regular grammar.
You must use DFA (Deterministic Finite Automata) or NFA (NondeterministicFinite Automata) to implement this program. The inputs of the program are atext document including a set of productions of 3º grammar (the regular grammar) and a source codetext document contains a set of strings to be recoganized. The output of theprogram is a token table which consists of 5 types token: key words, identifier,const, limiter and operator.
 The recommendation processing logic of the lexical analysis program: Generate NFA according to the regular grammar input by the user, and then generate DFA. Write a program that recognizes tokens based on DFA, identifies the source code entered by the user from left to right, and generates a token list Triples: line number, category, token content)
Requirements: The lexical analysis program can accurately identify constants in scientific notation (such as 0.314E + 1), complex constants (such as 10 + 12i), check the legality of integer output, and the legality of identifiers (the first character cannot be a number Etc.), try to comply with the rules of real high-level languages (such as C ++, Java or python)
任务2：创建一个使用LL(1) 方法或LR(1) 方法的语法分析程序。

程序有两个输入：1）一个是文本文档，其中包含2º型文法（上下文无关文法）的产生式集合；2）任务1词法分析程序输出的（生成的）token令牌表。程序的输出包括：YES或NO（源代码字符串符合此2º型文法，或者源代码字符串不符合此2º型文法）；错误提示文件，如果有语法错标示出错行号，并给出大致的出错原因。

语法分析程序的推荐处理逻辑：根据用户输入的2º型文法，生成Action及Goto表，设计合适的数据结构，判断 token序列（用户输入的源程序转换）是否符合LR1文法规则，识别token序列，输出结果。

提示：选用LR(1)方法的起评分数高



Task 2: Create a phaser analysis program,which uses LL(1) method or LR(1) method.

The program has two inputs: 1) a text document containing a production set of 2º-type grammars (context-free grammars); 2) a table of (generated) tokens output by the task 1 lexical analysis program. The output of the program includes: YES or NO (the source code string conforms to this 2º grammar, or the source code string does not conform to this 2º grammar); an error prompt file, if there is a syntax error, the error line number is indicated, and an approximate Cause of the error.

Recommended processing logic of the parser: Generate Action and Goto tables based on the 2º grammar input by the user, design the appropriate data structure, and determine whether the token sequence (source program conversion input by the user) conforms to the LR1 grammar rules, identify the token sequence, and output result.

Hint: The LR (1) method has a high starting score

（选作）任务3：创建符合属性文法规则的语义分析程序。
程序有两个输入：1）一个是文本文档，其中包含2º型文法（上下文无关文法+属性文法，包含语义规则注释，可以简单以表达式计算语义为例）的产生式集合；2）任务1词法分析程序输出的（生成的）token令牌表。
程序输出：四元式序列，可以利用优化技术生成优化后的四元式序列
提示：也可以利用Flex工具设计功能更加丰富的语义分析程序。
