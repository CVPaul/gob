## Debug
- `EventThread`中`run`需要是`private`属性的，否者调用`start`之后在`run`会出现死锁的问题