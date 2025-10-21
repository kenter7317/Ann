```
             ____   __       _                
   __ _  ___|___ \ / _| _   / \   _ __  _ __  
  / _` |/ _ \ __) | |(_|_) / _ \ | '_ \| '_ \ 
 | (_| |  __// __/|  _| _ / ___ \| | | | | | |
  \__,_|\___|_____|_|(_|_)_/   \_\_| |_|_| |_|
```

# ae2f::[Ann](https://codeberg.org/ae2f/Ann)
> Rel-v4.0.0

[![ae2f-workflow](https://github.com/ae2f/Ann/actions/workflows/cmake-test.yml/badge.svg?branch=main)](https://github.com/ae2f/Ann/actions/workflows/cmake-test.yml)

> This project has been migrated to [codeberg.org](https://codeberg.org/ae2f/Ann).  
> You will still be able to access to upstream with github page.

> ANN interfaces with basic implementation for example.  
> GPU acceleration backend is in plan to be shifted to vulkan.  

# Supported GPU Acceleration
> GPU acceleration via vulkan is now being implemented.  
> Configuring the project with `-Dae2fVK_needed=ON` will fetch vulkan as backend.  
> Here is the list of supported gpu acceleration.
- Slp/Mlp

# Dependency
## When external libraries has fullly compiled
- a functional OS
    - an Unix-based system is Recommended
    - Windows has been tested to work.

- a C compiler >= 90
- a builder (ninja, make, etc...)
- cmake >= 3.20

## When external libraries has not been fetched
> aka it's the first time configuring it
- git
- a functional network
- a DNS server for github

## When turning the develop mode on (ae2f_DEVEL)
- a C++ compiler >= 98

## With ae2fVK_needed
> aka you want GPU accelerated.

## When you don't have `clspv` and its dependencies installed and you're willing to fetch one automatically
- a C++ compiler >= 13
