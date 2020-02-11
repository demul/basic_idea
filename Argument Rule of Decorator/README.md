# argument_rule_of_decorator


파이썬 데코레이터가 전달인자들을 처리하는 순서에 대해 정리


```c
def deco_func1(arg1):
    print(arg1)
    def deco_func2(arg2):
        print(arg2)
        def deco_func3(arg3):
            print(arg3)
            # arg2(arg3)
        return deco_func3
    return deco_func2
```



위와 같이 데코레이터 함수를 선언한다.



```c
@deco_func1
def outer_func1(var):
    print('This is Outer Func1')
```



그리고 위와 같이 데코레이터를 이용해 함수 outer_func1을 선언한다.





```
[출력]
<function outer_func1 at 0x7f75b73854d0>
```





위와 같은 출력을 볼 수 있다. outer_func1을 선언하는 순간 deco_func1의 arg1에 outer_func1 함수의 포인터가 전달되고 print(arg1)이 실행되어 해당 포인터를 출력한다. 그리고 입력으로 arg2를 받고 출력하는 함수 deco_func2를 return한다. deco_func2의 포인터가 원래 outer_func1의 포인터를 대체한다.



```c
outer_func1('Hello World!')
```



이후 위와 같이 outer_func1을 call 했을 때, deco_func2가 실행되면서 arg2에 'Hello World!' 문자열을 전달받아 출력하면서,





```
[출력]
Hello World!
```





위와 같은 출력이 나온다. 기존의 outer_func1의 포인터가 deco_func2의 포인터로 대체되었으므로 "This is Outer Func1"은 출력되지 않는다.



그리고 deco_func3는 그저 선언에 그치게 되고 실행되지 않으므로 아무 기능도 하지 않는다. deco_func4도 마찬가지다.



여기까지는 다들 알고있을 자연스러운 decorator의 사용흐름이다. 이번엔 deco_func1에 명시적인 입력을 주면서 함수를 선언해보자.



```c
@deco_func1('This is Arg')
def outer_func2(var):
    print('This is Outer Func2')
```



위와 같이 deco_func1에 'This is Arg'라는 문자열을 입력으로 넣어주면서 함수 outer_func2를 선언한다.





```
[출력]
This is Arg
<function outer_func2 at 0x7fc3b7e95560>
```





outer_func2를 선언하는 순간 위와 같은 출력을 볼 수 있다. 출력이 두 줄로 늘었다. deco_func1의 arg1에는 'This is Arg'라는 문자열이 전달되고 바로 다음줄에서 출력된다.



그렇다면 함수 outer_func2가 전달되는 곳은 어디일까? 바로 deco_func2의 arg2이다. 위 출력에서 보이듯 print(arg2)라는 명령어에 의해 함수의 포인터가 출력된다.



이후 deco_func2 내부에 선언된 deco_func3의 포인터를 반환하여 outer_func2의 포인터를 대체한다. 



여기까지는 outer_func2를 선언할 때 일어나는 일이고, 이제 outer_func2를 call해보자.



```c
outer_func2('Hello World!')
```



위와 같이 outer_func2를 call하면 아래와 같은 출력을 볼 수 있다.





```
[출력]
Hello World!
```





outer_func2의 기존의 포인터는 deco_func3로 대체되었으므로 실제로 실행되는 것은 deco_func3이다. deco_func3는 arg3로 전달된 'Hello World!' 문자열을 출력한다.



즉 명시적인 입력이 있을때는, 함수의 선언시 decorator에 전달되는 순서와, 함수가 call됐을때 인자가 전달되는 순서가 한 칸씩 밀리게 됨을 확인할 수 있다.



최초 decorator 함수의 선언에서 주석처리한 부분을 제거해 다음과 같이 선언해보자



```c
def deco_func1(arg1):
    print(arg1)
    def deco_func2(arg2):
        print(arg2)
        def deco_func3(arg3):
            print(arg3)
            arg2(arg3) ## 주석 제거 됨
        return deco_func3
    return deco_func2

```



이후 아래와 같이 outer_func2를 선언 및 실행해보면 출력이 달라진 것을 확인 할 수 있다.



```c
@deco_func1('This is Arg')
def outer_func2(var):
    print('This is Outer Func2')


outer_func2('Hello World!')
```




```
[출력]
This is Arg
<function outer_func2 at 0x7f214e7b6290>
Hello World!
This is Outer Func2
```





'This is Outer Func2'라는 출력이 추가되었다. arg2(arg3)라는 명령어에 의해서다.



arg2에는 기존의 outer_func2의 포인터가 전달되고, arg3에는 함수가 call될 때 'Hello World!'라는 문자열이 전달되지만 outer_func2는 사실 입력을 무시하고 'This is Outer Func2'라는 문자열만을 출력하는 함수이므로, 'This is Outer Func2'가 출력된다.



그렇다면 아래 문장을 실행할려 하면 어떻게 될까?



```c
@deco_func1
def outer_func1(var):
    print('This is Outer Func1')


outer_func1('Hello World!')
```



언뜻보면 에러가 날 것 같아보인다. arg2에는 함수가 call될 때 'Hello World!'라는 문자열이 전달되는데, 이걸 arg2(arg3)같은 식으로 사용하려고 하니까 당연히 에러가 생길 것 같다. 그런데 막상 에러가 나지는 않는다.





```
[출력]
<function outer_func1 at 0x7f214e7e5a70>
Hello World!
```





에러가 나지 않는 이유는 위에서도 언급했지만 deco_func3가 그저 선언만 되고 사용되지 않았기 때문이다. 



```c
def deco_func1(arg1):
    print(arg1)
    def deco_func2(arg2):
        print(arg2)
        def deco_func3(arg3):
            print(arg3)
            arg2(arg3)
        deco_func3('This is Deco Func3') ## 추가된 부분
        return deco_func3
    return deco_func2
    
    
@deco_func1
def outer_func1(var):
    print('This is Outer Func1')


outer_func1('Hello World!')
```



위와 같이 deco_func3를 사용하려 하면 당연히 에러가 난다.



정리하자면 데코레이터는 다음과 같은 규칙을 가진다.







**1. 기본적으로 @ 연산자 옆에 오는 함수 A는 @ 연산자 밑에 선언된 함수 F의 포인터를 인자로 받는다.**


**2. @ 연산자 옆에 오는 함수 A에 명시적인 입력이 있을 땐, 해당 함수 내부에 선언된 함수 B가 @ 연산자 밑에 선언된 함수 F의 포인터를 인자로 받는다.**


**3. @ 연산자 옆에 오는 함수 A에 명시적인 입력이 있고, 함수 A 내부에 선언된 함수가 여러개면, 해당 함수들로 모두 @ 연산자 밑에 선언된 함수 F의 포인터가 전달된다.**
