# MyJVM in C++

## Environment
dev env: 
- **Linux** only
- $\geq$ **C++11**

## Modules so far
```bash
├── rtda
├── classfile
├── classpath
├── cmd
├── jre
└── utils
```

## Test
Each module has a '**test.cpp**' (except module utils)    
You can test each module use
```bash
make ${module_name}_test
./${module_name}_test
```

## Run JVM

```bash
make
./jvm -classpath src com.lyx.test
```

## Reference
[自己动手写java虚拟机 张秀宏著](%E8%87%AA%E5%B7%B1%E5%8A%A8%E6%89%8B%E5%86%99Java%E8%99%9A%E6%8B%9F%E6%9C%BA.pdf)