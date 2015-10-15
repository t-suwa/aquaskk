aquaskk
=======

An input method without morphological analysis.

## How to build

### Build package/dmg
```
$ cd aquaskk/platform/mac
$ make pkg
$ make dmg
```

### Install debug build
After install AquaSKK package, you could replace `AquaSKK.app` with debug build.

```
$ cd aquaskk/platform/mac
$ make install
```

You might need logout/login to start new AquaSKK process.

### Install release build
After install AquaSKK package, you could replace `AquaSKK.app` with release build.

```
$ cd aquaskk/platform/mac
$ make install-rc
```

You might need logout/login to start new AquaSKK process.

# Troubleshoot
## Use with iTerm2/Apple Terminal.app
see http://mzp.hatenablog.com/entry/2015/03/15/213219
