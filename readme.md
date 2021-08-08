### ycit-judger
1. compile: `cd ./src && clang++ *.cpp -lpthread -o judger -std=c++17`
2. run: judger memory_limit time_limit  lang input_path output_path source_path submisson gen_path

| param name   | type   | remark                                         |
| ------------ | ------ | ---------------------------------------------- |
| memory_limit | int    | MB                                             |
| time_limit   | int    | ms                                             |
| lang         | string | none                                           |
| input_path   | string | none                                           |
| output_path  | string | none                                           |
| source_path  | string | the file you need to compiler/interpret && run |
| submisson    | string | to gen executable filename                     |
| gen_path     | string | the executable file will put into gen_path     |

here are some examples
```bash
create test/xxx and test/1.in 
cpp: ./judger 10 1000 cpp ../test/1.in ../test/1.out ../test/main.cpp 111 ../test/
merdog: ./judger 10 1000 merdog ../test/1.in ../test/1.out ../test/main.mer 111 ../test/
python: ./judger 10 1000 python ../test/1.in ../test/1.out ../test/main.py 111 ../test/
JS: ./judger 10 1000 node ../test/1.in ../test/1.out ../test/main.js 111 ../test/
```
3. return state:

write JudgeData to a named pipe
Write content: 
```cpp
struct JudgerData{
    int usage_time;
    int usage_mem;
};
```
pipe_name: `"/tmp/pipe"+info.submission_id`

| return value | remark       |
| ------------ | ------------ |
| -1           | judger_error |
| 0            | Okay         |
| 1            | TLE          |
| 2            | MLE          |
| 3            | RE           |
| 4            | UKE          |
| 5            | CE           |

### Something you need to take care.
* **MLE is likely to be mistaken for RE**
* If you need to add new languages or update compile/interpreter, you should change run_code.cpp -> interpretor_arg / run_code-> compiler_arg
* The ans file(the .out file that submmit code gened.) will write into the folder you specified.
