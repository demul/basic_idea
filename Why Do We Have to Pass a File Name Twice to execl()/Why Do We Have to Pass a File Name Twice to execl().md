# Why Do We Have to Pass a File Name Twice to execl()?

## 문제

리눅스에서 현재의 프로세스를 다른 프로세스로 전환할 때 exec계열의 명령어를 사용한다.



이중 접미에 l이 붙은 계열은 실행할 명령어의 인자를 매개변수의 형태로 넘겨준다.(v가 붙은 계열은 인자들을 배열의 형태로 넘겨준다.)

```c
int execl(const char *path, const char* arg0..., NULL);
```
여기까지는 시스템 프로그래밍을 공부한 사람이라면 다들 알고있는 사실이다. 내가 하게된 고민은 예를 들어 '/bin/ls /home' 명령어를 실행하고자 할 때,


```c
int execl('/bin/ls', '/bin/ls', '/home', NULL);
```
왜 이런 식으로 '/bin/ls'를 두 번씩이나 넘겨주는가 하는 것이다.



## 해결

구글링 결과 다음과 같은 [설명](https://unix.stackexchange.com/questions/187666/why-do-we-have-to-pass-the-file-name-twice-in-exec-functions)을 찾아 볼 수 있었다.



요약하자면 execl()의 첫 번째 인자는 **실행파일의 경로**를 나타내는 것이고 두 번째 인자는 실질적으로 **실행파일에 넘겨주는 실행파일명**이다.



**실행파일에 넘겨주는 실행파일명**이라는 설명이 와닿지 않을 수 있는데 간단히 말하면 main 함수의 매개변수 argv[0]에 전달되는 인자를 말한다.



여기까지 오니까, 예전 main 함수에 인자를 넘겨주는 방법을 처음 배웠을 때, 왜 argv[0]가 필요하냐고 질문했던 기억이 난다. 교수님께 "C언어 하다보면 실행파일명이 필요할 때가 있는데, 그게 뭔지는 스스로 찾아봐라." 정도의 답변을 받고 이러한 함수 설계의 철학에 대해 더 이상 의심하기를 멈추었던 기억이 난다.(왜냐면 스스로 찾아보기 귀찮았기 때문에)



마침내 오늘에서야 '실행파일명이 필요할 때' 중 하나를 알게되었다. **Symbolic Link를 이용해서 Alias를 구현할 때 실행파일명을 구분자로 사용할 수 있다!** 여기서 주의해야 할 점은 Symbolic Link를 이용해서 구현된 Alias는 Shell단에서 지원되는 Alias 기능(터미널에서 $alias 명령어로 확인할 수 있는 것들)과는 그 원리가 전혀 다른 것이라는 점이다.



Shell단에서 지원되는 Alias 기능이 일종의 매크로라면, Symbolic Link를 이용해서 구현하는 Alias는 원본을 실행했을 때와 Symbolic Link를 실행했을 때 main 함수에 전달되는 실행파일명(argv[0])이 다르다는 점을 이용, 함수자체가 argv[0]를 구분자로 사용해서 서로 다른 기능을 수행하게끔 만드는 것이다. 이 말이 사실인지 알아보기 위해 예시를 살펴보자.



위 [링크](https://unix.stackexchange.com/questions/187666/why-do-we-have-to-pass-the-file-name-twice-in-exec-functions)에서 들고있는 예시는 다음과 같다.




'Some applications do, however, make use of argv[0]. Usually these have one or more symlinks in $PATH; this is common with compression utilities (sometimes they use shell wrappers instead). If you have xz installed, stat $(which xzcat) shows it's a link to xz, and man xzcat is the same as man xz which explains "xzcat is equivalent to xz --decompress --stdout". The way xz can tell how it was invoked is by checking argv[0], making these equivalent'




xz라는 압축유틸을 예시로 들고있다. 우분투에는 기본으로 내장되어 있는 유틸리티인데 소스코드는 홈페이지(<http://tukaani.org/xz/>)에서 직접 다운받아야한다. 현재 최신버전은 5.2.4이다.



소스코드를 다운받아서 압축을 푼 뒤, 'xz-5.2.4\xz-5.2.4\src\xz' 경로로 가서, 냅다 main.c 파일을 열어보자. 우리가 관심있는 것은 main 함수의 입력인자이므로 main함수로 가서 argv라는 키워드로 검색을 해보면 아래 두 함수에서 argv를 입력인자로 받고 있음을 알수 있다.



![img](Images/img1.JPG)



tuklib_progname_init()은 'xz-5.2.4\xz-5.2.4\src\xz\tuklib_progname.c'에 정의되어 있는데, 단순 경로명 strip 함수이다.(C는 이런 것도 직접 만들어야 하나...?)



우리가 관심있게 봐야할 것은 main.c와 같은 디렉토리에 args.c에 정의되어 있는 args_parse()이다.



![img](Images/img2.JPG)



위와 같이 argv[0](헷갈릴까봐 다시 한 번 강조하자면 **실제 실행시 사용하는 파일명**)을 구분자로 삼아 옵션을 달리주는 모습을 확인할 수 있다.



여기까지 이해했다면 직접 Symbolic Link를 이용한 Alias를 구현해볼 수 있다.

```c
#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
char* strstrip(char* str)
{
	// Strip the leading path.
	char *p = str + strlen(str);
	while (str < p && p[-1] != '/' && p[-1] != '\\')
		--p;
 	str = p;

 	// Strip the .exe suffix.
	p = strrchr(p, '.');
	if (p != NULL)
		*p = '\0';
 	return str;
} 

int main(int argc, char* argv[])
{
	if(argc != 1)
	{
		printf("Do not need additional argument\n");
		exit(1);
	}

	char* str = strstrip(argv[0]);
 	if(strcmp(str, "hello")!=0)
	{
		printf("I am Hello%d\n", atoi(str+5));
		//hello2 will print "I am Hello2"
	}
	else
	{
		printf("Hello world\n");
	}

	return 0;
}
```
본 repository에 같이 첨부된 hello.c 파일의 소스코드다.



위 코드를 컴파일해서 hello라는 이름의 실행파일을 만든 뒤, hello2라는 이름의 Symbolic Link를 하나 걸어준 뒤 실행해보자



이미지



위와 같은 출력을 볼 수 있다. 



마무리로 본 repository에 같이 첨부된 execl_.c를 살펴보도록 하자.

```c
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
extern char **environ;
int main(int argc, char* argv[])
{
	if(argc != 3)
	{
		printf("Usage %s <1st arg> <2st arg>\n", argv[0]);
		exit(1);
	}

        pid_t pid;
        int ret;

        ret = execlp(argv[1], argv[2], NULL);
        fprintf(stderr, "execl error: %s\n",strerror(errno));
        printf("execl:%d\n",ret);
        return 0;
}
```
첫번째 인자로 실행파일의 경로, 두번째 인자로 실행파일의 main 함수에 argv[0]의 인자로 전해질, 실제로 실행할 때 사용한 파일명을 입력받는 프로그램이다. 실행결과는 다음과 같다.



이미지



이러한 결과로부터 우리는 exec 계열 함수들의 동작방식 뿐만 아니라, 터미널에서 파일을 실행할 때 실제로 어떤 방식으로 실행파일이 호출되는지 어느정도 이해할 수 있다.



Symbolic Link는 특정 경로를 지정하는 포인터에 불과하다. 하지만 터미널상에선 우리가 무슨 이름으로 명령어를 실행했는지가 중요하고, 이것은 main 함수의 인자로 전달된다.



그런데 위와 같은 결론으로 하나의 고민은 해결됐지만 또다른 고민이 꼬리를 잇는다. Symbolic Link가 단지 편의성을 위해 제공되는 기능인줄 알았는데 이런 용도가 있었다니... C++같은게 생기기 이전 시절에, 애초부터 Alias도 염두에 두고 설계된 기능일까? 



아니면 경로명과 실행파일명을 분리하는 데는 좀 더 근본적인 이유와 철학이 있고, Symbolic Link를 통한 Alias 기능은 그 과정에서 생긴 부산물일뿐일까? 리눅스 시스템은 공부하면 할수록 오묘한 것 같다.
