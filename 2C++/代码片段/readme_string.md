# string中是否含有某个字符

```c++
bool contains = (t.find(x) != std::string::npos);
```

```c++
#include <algorithm>
bool contains = (std::find(t.begin(), t.end(), x) != t.end());
```





