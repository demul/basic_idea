def deco_func1(arg1):
    print(arg1)
    def deco_func2(arg2):
        print(arg2)
        def deco_func3(arg3):
            print(arg3)
            # arg2(arg3)
        # deco_func3('This is Deco Func3')
        return deco_func3
    return deco_func2



@deco_func1
def outer_func1(var):
    print('This is Outer Func1')


outer_func1('Hello World!')


print('===========================')

@deco_func1('This is Arg')
def outer_func2(var):
    print('This is Outer Func2')


outer_func2('Hello World!')