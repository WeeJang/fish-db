# 这是为毕业设计开发一个KG-engine


 
  


## reference:
  《基于压缩位图索引的RDF数据存储与管理》- 姜伟 北京交通大学



# FishDB

 这是为硕士毕业设计开发一个知识图库查询引擎（KG-Engine)。全部由C++实现，支持SPARQL语句的查询。索引的实现使用了[Roarning](https://github.com/RoaringBitmap/CRoaring)的压缩位图算法。数据底层存储，SPARQL语句解析，及查询算法均自己开发实现。
  在此感谢DingfuData的工匠级工程师Chen Rushan给我毕业设计提供的指导和帮助，是他的技术热情和态度让我实习中看到了一位优秀的工程师应该具备怎样的素质，并一直是我工作上的榜样。感谢我的女朋友Fish，这段时间来给我学业上的鼓励和支持，引擎名字来自于她的英文名字，希望她能够在读博期间自由自在的享受科研。

## feature:

1. 基于压缩位图索引
2. 底层数据分块，顺序存储（参考HBase），便于后期水平扩展
3. 支持基本的SPARQL语句查询
4. 查询算法独立设计 

## performance:

1.在 no-cache 情况下， fishdb 相比较 Cayley 而言拥有 1.5 个数量级的响应性能提升，付出的代价是在初始化时需要多占用 0.9 个数量级的内存消耗
2.在 cache 情况下，fishdb 查询计算时需要消耗 3%的增量内存空间，而 Cayley则需要多消耗 11%的增量内存空间。如果在不考虑初始内存占用的情况下，假设两者的响应速度相同，则 fishdb 系统的吞吐量将是 Cayley 的近 3倍；同时通过测试可以看到， fishdb 在 cache 情况下还具有 0.5 个数量级的响应速度优势，因此系统吞吐量的优势更为明显


## Getting Started

### Prerequisites

1.代码是基于C++11开发实现的，因此安装环境下的C++编译器需要支持C++11.

2.工程编译工具使用的[CMake](https://cmake.org/),建议版本>=3.7.2

3.第三方库使用了[Boost](http://www.boost.org/doc/libs/1_65_1/more/getting_started/index.html)，建议版本>=1.63.0


### Installing

```
git clone https://github.com/WeeJang/fish-db.git
cd fish-db
cmake
make
```

End with an example of getting some data out of the system or using it for a little demo

## Running the tests

测试case没有形成体系，只有单个的功能测试，这一部分需要优化。

## Deployment

Add additional notes about how to deploy this on a live system

## Built With

* [CMake](https://cmake.org/) - open-source, cross-platform family of tools designed to build, test and package software
* [Boost](https://maven.apache.org/) - C++ source libraries


## reference:
  《基于压缩位图索引的RDF数据存储与管理》- 姜伟 北京交通大学

## Authors

* **Jang Wee** - *Initial work* - [JangWee](https://github.com/WeeJang/)


## License

This project is licensed under the MIT License.

## Acknowledgments

