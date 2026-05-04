# pack_cli build notes

## VS2003
- Open `pack_cli.vcproj` in Visual Studio .NET 2003.
- Build dependencies first:
  1. `..\zlib\zlib.vcproj` (Debug/Release)
  2. `..\engine\Engine.vcproj` (Debug/Release)
- Then build `pack_cli` with matching configuration.

Library search paths are preconfigured:
- Debug: `..\engine\Debug`, `..\zlib\Debug`
- Release: `..\engine\Release`, `..\zlib\Release`

## VS2015
- Open `pack_cli_vs2015.sln`.
- Retarget toolset/SDK if prompted.
- Build `zlib` and `engine` first (or ensure `.lib` files already exist in configured directories).
- Build `pack_cli`.

## CLI usage
```bash
pack_cli.exe --input D:\work\datas --output D:\client\data\datas.pak --password 20210112taozhi --zip 1 --encrypt 1 --level 9
```

- `--password` omitted => defaults to legacy password from `PackAggregate::GetDefaultPassword()`.
- `--zip` `1|0`
- `--encrypt` `0|1|2`
- `--level` `0..9`
