aquaskk
=======

An input method without morphological analysis.

## How to build

### Build package/dmg
Install [Iceberg](http://s.sudre.free.fr/Software/Iceberg.html).  And run `/Library/StartupItems/IcebergControlTowerTool` as root.

```
$ cd aquaskk/platform/mac
$ make pkg
$ make dmg
```

### Instal debug build
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
