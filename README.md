## neverrun

Neverrun is a module/server launcher/browser written in Qt.  See the
neverwintervault.org entry for details
[here](http://neverwintervault.org/project/nwn1/other/tool/neverrun).

Neverrun also allows servers to use [Markdown](http://daringfireball.net/projects/markdown/syntax) (with some extensions) in both
their module description and ServerDesc.txt.  Since this is ultimately rendered by WebKit it should greatly improve things
for non-English languages.

Extensions Available:

* Tables
* Auto-linking URLs
* Strikethrough

ServerDesc.txt can also have a YAML metadata header at the beginning of the file.  The current supported keys are

* `homepage` - Link to your server's homepage.
* `forum` - Link to your server's forum.
* `webchat` - Link to your server's web chat.

Note this is optional.  It does, however, give options to neverrun users for easily exploring your server's
community.  If you have any suggestions for further additions, please feel free to send me a message.

Example:

```yaml
---
website: http://google.com
forum: http://forum.bioware.com/forum/43-neverwinter-nights/
webchat: https://kiwiirc.com/
---
```

You can see a larger example [here](https://github.com/jd28/neverrun/blob/master/examples/ServerDesc.txt).

There are a number of [online markdown editors](http://dillinger.io/) that can help gain a better understanding of
the format.

### 3rd Party libraries/code/resources:
* [QDarkStyleSheet](https://github.com/ColinDuquesnoy/QDarkStyleSheet)
* [CuteMarkEd](https://github.com/cloose/CuteMarkEd) - Markdown converter.
* [hoedown](https://github.com/hoedown/hoedown)
* [Fervor](https://github.com/pypt/fervor) - Auto-updater.
* [yaml-cpp](https://code.google.com/p/yaml-cpp/)
