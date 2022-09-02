#include<stdio.h>

int a=2;
int b=4;

void test(int *p){
    
    int *h;
    printf("%d, %p, %p\r\n",*p,p,h);
    h = p;
    *h+=1;
    printf("%d, %p, %p\r\n",*p,p,h);
    h = &a;
    *h +=1;
    printf("%d, %p, %p\r\n",*p,p,h);
    h = &b;
    *h +=1;
    printf("%d, %p, %p\r\n",*p,p,h);//ここと最後はhのアドレス一緒
    p = h;
    //これで書き変わるかどうか.アドレスそれ自体を保存しておいて、
    printf("%d, %p, %p\r\n",*p,p,h); //値はここだけ変わった
}
int main()
{
    //関数内でアドレスを渡したときに、渡した側の変数の内容が変わってほしくない
    int c=1;
    test(&c);
    // int *b = &a;
    // int c = a;
    // printf("%d,%p,%d,%p,%d,%p\r\n",a,&a,*b,b,c,&c);//cだけアドレスが違う
    // *b = 2;
    // printf("%d,%p,%d,%p,%d,%p\r\n",a,&a,*b,b,c,&c);
    return 0;
}