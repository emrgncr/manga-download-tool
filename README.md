# Manga Download Tool
This is a desktop app that downloads mangas as pdf.
## Running
On linux, you can either download the AppImage file from Releases and use it or build from source.

To use the AppImage
```
chmod +x Manga_Download_Tool-x86_64.AppImage
./Manga_Download_Tool-x86_64.AppImage
```
## Building
### Linux
You will need qmake and libhpdf in addition to basic build tools.

To instal libhpdf:

Ubuntu:
```
apt install libhpdf-2.3.0
```
Fedora:
```
dnf install libharu
```
Or you can build it yourself: https://github.com/libharu/libharu


Then clone the repository, cd into it. 
```
git clone https://github.com/emrgncr/manga-download-tool.git
cd manga-download-tool/
qmake && make
```
Should be able to build it.
### Windows
I have no idea.
