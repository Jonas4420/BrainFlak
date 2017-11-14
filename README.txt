================================================================================
== NILADS
================================================================================

	There are two types of functions: Nilads and Monads. A nilad is a function that takes 0 arguments. Here are all of the nilads:
	
	- () Evaluates to one.
	- [] Evaluates to the height of the current stack.
	- {} Pop the active stack. Evaluates to the popped value.
	- <> Toggle the active stack. Evaluates to zero.
	
	These are added together when they are evaluated. So if we had a '3' on top of the active stack, this snippet:
	
	()(){}
	
	would evaluate to 1 + 1 + active.pop() which would evaluate to 5.

================================================================================
== MONADS
================================================================================

	The monads take one argument, a chunk of Brain-Flak code. Here are all of the monads:
	
	- (n) Push 'n' on the active stack.
	- [n] Evaluates to negative 'n'
	- {foo} While zero is not on the top of the stack, do foo.
	- <foo> Execute foo, but evaluate it as 0.
	
	These functions will also return the value inside of them, so
	
	(()()())
	
	Will push 3 but
	
	((()()()))
	
	Will push 3 twice.
	
	The `{}` will evaluate to the sum of all runs. So if we had '3' and '4' on the top of the stack:
	
	{{}}
	
	would evaluate as 7.
	
	When the program is done executing, each value left on the active stack is printed, with a newline between. Values on the other stack are ignored.
	
	That's it. That's the whole language. 

================================================================================
== SAMPLE CODE
================================================================================

	A larger variety of up to date code samples can be found in the Brain-Flak wiki.
	Add two numbers
	
	({}{})
	
	Sum of all inputs
	
	(([]){[{}]{}([])}{})
	
	Fibonacci
	
	<>((()))<>{({}[()])<>({}<>)<>(({})<>({}<>))<>}<>{}{}
	
	Integer division.
	
	This is the shortest known integer division program that works for all non-zero integers at 246 characters.
	
	(()()){({}[()]<([([({})](<()>))](<>())){({}())<>}{}<>{}{}<>(({})){{}{}<>(<(())>)}{}(({})<>)<>{(<{}([{}])>)}{}({}<>)<>({}<><({}<>)>)>)}{}({}(<>))<>([()]{()<(({})){({}[()])<>}{}>}{}<><{}{}>)<>(({}{}[(())])){{}{}(((<{}>)))}{}{}{(<{}<>([{}])><>)}{}<>
