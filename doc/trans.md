# Trans模块

>  涉及到的头文件 `libnap.h`

Trans模块用于base16和base64转换。

> 详细类定义请见  `src/trans.h`

<br/>
<br/>

## BASE16（Hex）

```c++
/*
	字符串转Hex
*/
Hex hex;
binstream msg("helloworld");
binstream hexmsg = hex.toHex(msg, false); // 第二个参数值Hex的大小写
cout << hexmsg;

/*
	将Hex转为原始字符串
*/
binstream bin = hex.toBinary(hexmsg);
cout << bin;
```

<br/>

<br/>

## BASE64（Hex）

```c++
binstream str("hello world");
bool safe = true; //url safe

/*
	将字符串转Base64
	第二个参数指明是否使用 urlsafe 模式
*/
binstream b64 = Base64::encode(str, safe);
cout<<b64<<endl;

/*
	将Base64转为原始字符串
	第二个参数指明是否使用 urlsafe 模式
*/
cout<< Base64::decode(b64, safe);
```

