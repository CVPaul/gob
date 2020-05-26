### About `GOB`

- `GOB` is the abbreviation of `Gate Of Babylon` -- 《Fate》
- Its goal is to precipitate some useful tools in all the projects

### Module: `log`

- this is the first member of `gob` and I had upload to [git](https://github.com/CVPaul/logger) at an early time, and since I am Chinese so the `README.md` was also write in Chinese at that time
- In short, this module is an out-of-the-box boost-based log library

#### New Features
- last days I compared my lib with `glog`(google logger library) and found that `glog` is faster than this one in `sync-mode` and about the same with this one in `async-mode`,
on the other hand `glog` is much lighter than `boost` so I decided to include `glog` in my project
- if `USING_GLOG` marco is define then this lib will using `glog` as basic, more detail can be found in `CMakeLists.txt`
- rolling feature was also implemented

### Module: `event`
- this is a C++ implement of `Producer/Consumer Model` and it has following submodules
    - `event_thread`: a multi producer one consumer model in pure C++
    - `event_multi_thread`: a multi producer multi consumer model bases on boost::asio and this implement have some extenal options: `using strand or not`, `post with dispatch or not`
    - `multi_thread_loop`: a multi producer multi consumer model in pure C++
- all the submodules have the same apis, so you can easily switch without cost
- all the sunmodules can be runing like a service (never exit unless `exit` called), with is the reason I do not using `boost::asio::threadpool` or some other `threadpool implement` like [this](http://threadpool.sourceforge.net/)
- the recomend is `multi_thread_loop` which a little faster than `boost::asio` in the `unit_test` case and it is written in pure C++ this means everything is in control

### Others: 
- Thirdparty Useful Tools
    - [Remotery](https://github.com/Celtoys/Remotery): A realtime CPU/GPU profiler hosted in a single C file with a viewer that runs in a web browser.