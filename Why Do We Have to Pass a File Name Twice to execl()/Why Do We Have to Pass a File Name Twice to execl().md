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



구글링 결과 다음과 같은 [설명](https://unix.stackexchange.com/questions/187666/why-do-we-have-to-pass-the-file-name-twice-in-exec-functions)을 찾아 볼 수 있었다.



요약하자면 execl()의 첫 번째 인자는 **실행파일의 경로**를 나타내는 것이고 두 번째 인자는 실질적으로 **실행파일에 넘겨주는 실행파일명**이다.
