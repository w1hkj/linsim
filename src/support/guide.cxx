const char* szGuide = 
"<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.1//EN\"\n\
    \"http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd\">\n\
<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"en\">\n\
<head>\n\
<meta http-equiv=\"Content-Type\" content=\"application/xhtml+xml; charset=UTF-8\" />\n\
<meta name=\"generator\" content=\"AsciiDoc 8.6.6\" />\n\
<title>Guide to linsim</title>\n\
<style type=\"text/css\">\n\
/* Shared CSS for AsciiDoc xhtml11 and html5 backends */\n\
\n\
/* Default font. */\n\
body {\n\
  font-family: Georgia,serif;\n\
}\n\
\n\
/* Title font. */\n\
h1, h2, h3, h4, h5, h6,\n\
div.title, caption.title,\n\
thead, p.table.header,\n\
#toctitle,\n\
#author, #revnumber, #revdate, #revremark,\n\
#footer {\n\
  font-family: Arial,Helvetica,sans-serif;\n\
}\n\
\n\
body {\n\
  margin: 1em 5% 1em 5%;\n\
}\n\
\n\
a {\n\
  color: blue;\n\
  text-decoration: underline;\n\
}\n\
a:visited {\n\
  color: fuchsia;\n\
}\n\
\n\
em {\n\
  font-style: italic;\n\
  color: navy;\n\
}\n\
\n\
strong {\n\
  font-weight: bold;\n\
  color: #083194;\n\
}\n\
\n\
h1, h2, h3, h4, h5, h6 {\n\
  color: #527bbd;\n\
  margin-top: 1.2em;\n\
  margin-bottom: 0.5em;\n\
  line-height: 1.3;\n\
}\n\
\n\
h1, h2, h3 {\n\
  border-bottom: 2px solid silver;\n\
}\n\
h2 {\n\
  padding-top: 0.5em;\n\
}\n\
h3 {\n\
  float: left;\n\
}\n\
h3 + * {\n\
  clear: left;\n\
}\n\
h5 {\n\
  font-size: 1.0em;\n\
}\n\
\n\
div.sectionbody {\n\
  margin-left: 0;\n\
}\n\
\n\
hr {\n\
  border: 1px solid silver;\n\
}\n\
\n\
p {\n\
  margin-top: 0.5em;\n\
  margin-bottom: 0.5em;\n\
}\n\
\n\
ul, ol, li > p {\n\
  margin-top: 0;\n\
}\n\
ul > li     { color: #aaa; }\n\
ul > li > * { color: black; }\n\
\n\
pre {\n\
  padding: 0;\n\
  margin: 0;\n\
}\n\
\n\
#author {\n\
  color: #527bbd;\n\
  font-weight: bold;\n\
  font-size: 1.1em;\n\
}\n\
#email {\n\
}\n\
#revnumber, #revdate, #revremark {\n\
}\n\
\n\
#footer {\n\
  font-size: small;\n\
  border-top: 2px solid silver;\n\
  padding-top: 0.5em;\n\
  margin-top: 4.0em;\n\
}\n\
#footer-text {\n\
  float: left;\n\
  padding-bottom: 0.5em;\n\
}\n\
#footer-badges {\n\
  float: right;\n\
  padding-bottom: 0.5em;\n\
}\n\
\n\
#preamble {\n\
  margin-top: 1.5em;\n\
  margin-bottom: 1.5em;\n\
}\n\
div.imageblock, div.exampleblock, div.verseblock,\n\
div.quoteblock, div.literalblock, div.listingblock, div.sidebarblock,\n\
div.admonitionblock {\n\
  margin-top: 1.0em;\n\
  margin-bottom: 1.5em;\n\
}\n\
div.admonitionblock {\n\
  margin-top: 2.0em;\n\
  margin-bottom: 2.0em;\n\
  margin-right: 10%;\n\
  color: #606060;\n\
}\n\
\n\
div.content { /* Block element content. */\n\
  padding: 0;\n\
}\n\
\n\
/* Block element titles. */\n\
div.title, caption.title {\n\
  color: #527bbd;\n\
  font-weight: bold;\n\
  text-align: left;\n\
  margin-top: 1.0em;\n\
  margin-bottom: 0.5em;\n\
}\n\
div.title + * {\n\
  margin-top: 0;\n\
}\n\
\n\
td div.title:first-child {\n\
  margin-top: 0.0em;\n\
}\n\
div.content div.title:first-child {\n\
  margin-top: 0.0em;\n\
}\n\
div.content + div.title {\n\
  margin-top: 0.0em;\n\
}\n\
\n\
div.sidebarblock > div.content {\n\
  background: #ffffee;\n\
  border: 1px solid #dddddd;\n\
  border-left: 4px solid #f0f0f0;\n\
  padding: 0.5em;\n\
}\n\
\n\
div.listingblock > div.content {\n\
  border: 1px solid #dddddd;\n\
  border-left: 5px solid #f0f0f0;\n\
  background: #f8f8f8;\n\
  padding: 0.5em;\n\
}\n\
\n\
div.quoteblock, div.verseblock {\n\
  padding-left: 1.0em;\n\
  margin-left: 1.0em;\n\
  margin-right: 10%;\n\
  border-left: 5px solid #f0f0f0;\n\
  color: #888;\n\
}\n\
\n\
div.quoteblock > div.attribution {\n\
  padding-top: 0.5em;\n\
  text-align: right;\n\
}\n\
\n\
div.verseblock > pre.content {\n\
  font-family: inherit;\n\
  font-size: inherit;\n\
}\n\
div.verseblock > div.attribution {\n\
  padding-top: 0.75em;\n\
  text-align: left;\n\
}\n\
/* DEPRECATED: Pre version 8.2.7 verse style literal block. */\n\
div.verseblock + div.attribution {\n\
  text-align: left;\n\
}\n\
\n\
div.admonitionblock .icon {\n\
  vertical-align: top;\n\
  font-size: 1.1em;\n\
  font-weight: bold;\n\
  text-decoration: underline;\n\
  color: #527bbd;\n\
  padding-right: 0.5em;\n\
}\n\
div.admonitionblock td.content {\n\
  padding-left: 0.5em;\n\
  border-left: 3px solid #dddddd;\n\
}\n\
\n\
div.exampleblock > div.content {\n\
  border-left: 3px solid #dddddd;\n\
  padding-left: 0.5em;\n\
}\n\
\n\
div.imageblock div.content { padding-left: 0; }\n\
span.image img { border-style: none; }\n\
a.image:visited { color: white; }\n\
\n\
dl {\n\
  margin-top: 0.8em;\n\
  margin-bottom: 0.8em;\n\
}\n\
dt {\n\
  margin-top: 0.5em;\n\
  margin-bottom: 0;\n\
  font-style: normal;\n\
  color: navy;\n\
}\n\
dd > *:first-child {\n\
  margin-top: 0.1em;\n\
}\n\
\n\
ul, ol {\n\
    list-style-position: outside;\n\
}\n\
ol.arabic {\n\
  list-style-type: decimal;\n\
}\n\
ol.loweralpha {\n\
  list-style-type: lower-alpha;\n\
}\n\
ol.upperalpha {\n\
  list-style-type: upper-alpha;\n\
}\n\
ol.lowerroman {\n\
  list-style-type: lower-roman;\n\
}\n\
ol.upperroman {\n\
  list-style-type: upper-roman;\n\
}\n\
\n\
div.compact ul, div.compact ol,\n\
div.compact p, div.compact p,\n\
div.compact div, div.compact div {\n\
  margin-top: 0.1em;\n\
  margin-bottom: 0.1em;\n\
}\n\
\n\
tfoot {\n\
  font-weight: bold;\n\
}\n\
td > div.verse {\n\
  white-space: pre;\n\
}\n\
\n\
div.hdlist {\n\
  margin-top: 0.8em;\n\
  margin-bottom: 0.8em;\n\
}\n\
div.hdlist tr {\n\
  padding-bottom: 15px;\n\
}\n\
dt.hdlist1.strong, td.hdlist1.strong {\n\
  font-weight: bold;\n\
}\n\
td.hdlist1 {\n\
  vertical-align: top;\n\
  font-style: normal;\n\
  padding-right: 0.8em;\n\
  color: navy;\n\
}\n\
td.hdlist2 {\n\
  vertical-align: top;\n\
}\n\
div.hdlist.compact tr {\n\
  margin: 0;\n\
  padding-bottom: 0;\n\
}\n\
\n\
.comment {\n\
  background: yellow;\n\
}\n\
\n\
.footnote, .footnoteref {\n\
  font-size: 0.8em;\n\
}\n\
\n\
span.footnote, span.footnoteref {\n\
  vertical-align: super;\n\
}\n\
\n\
#footnotes {\n\
  margin: 20px 0 20px 0;\n\
  padding: 7px 0 0 0;\n\
}\n\
\n\
#footnotes div.footnote {\n\
  margin: 0 0 5px 0;\n\
}\n\
\n\
#footnotes hr {\n\
  border: none;\n\
  border-top: 1px solid silver;\n\
  height: 1px;\n\
  text-align: left;\n\
  margin-left: 0;\n\
  width: 20%;\n\
  min-width: 100px;\n\
}\n\
\n\
div.colist td {\n\
  padding-right: 0.5em;\n\
  padding-bottom: 0.3em;\n\
  vertical-align: top;\n\
}\n\
div.colist td img {\n\
  margin-top: 0.3em;\n\
}\n\
\n\
@media print {\n\
  #footer-badges { display: none; }\n\
}\n\
\n\
#toc {\n\
  margin-bottom: 2.5em;\n\
}\n\
\n\
#toctitle {\n\
  color: #527bbd;\n\
  font-size: 1.1em;\n\
  font-weight: bold;\n\
  margin-top: 1.0em;\n\
  margin-bottom: 0.1em;\n\
}\n\
\n\
div.toclevel1, div.toclevel2, div.toclevel3, div.toclevel4 {\n\
  margin-top: 0;\n\
  margin-bottom: 0;\n\
}\n\
div.toclevel2 {\n\
  margin-left: 2em;\n\
  font-size: 0.9em;\n\
}\n\
div.toclevel3 {\n\
  margin-left: 4em;\n\
  font-size: 0.9em;\n\
}\n\
div.toclevel4 {\n\
  margin-left: 6em;\n\
  font-size: 0.9em;\n\
}\n\
\n\
span.aqua { color: aqua; }\n\
span.black { color: black; }\n\
span.blue { color: blue; }\n\
span.fuchsia { color: fuchsia; }\n\
span.gray { color: gray; }\n\
span.green { color: green; }\n\
span.lime { color: lime; }\n\
span.maroon { color: maroon; }\n\
span.navy { color: navy; }\n\
span.olive { color: olive; }\n\
span.purple { color: purple; }\n\
span.red { color: red; }\n\
span.silver { color: silver; }\n\
span.teal { color: teal; }\n\
span.white { color: white; }\n\
span.yellow { color: yellow; }\n\
\n\
span.aqua-background { background: aqua; }\n\
span.black-background { background: black; }\n\
span.blue-background { background: blue; }\n\
span.fuchsia-background { background: fuchsia; }\n\
span.gray-background { background: gray; }\n\
span.green-background { background: green; }\n\
span.lime-background { background: lime; }\n\
span.maroon-background { background: maroon; }\n\
span.navy-background { background: navy; }\n\
span.olive-background { background: olive; }\n\
span.purple-background { background: purple; }\n\
span.red-background { background: red; }\n\
span.silver-background { background: silver; }\n\
span.teal-background { background: teal; }\n\
span.white-background { background: white; }\n\
span.yellow-background { background: yellow; }\n\
\n\
span.big { font-size: 2em; }\n\
span.small { font-size: 0.6em; }\n\
\n\
span.underline { text-decoration: underline; }\n\
span.overline { text-decoration: overline; }\n\
span.line-through { text-decoration: line-through; }\n\
\n\
\n\
/*\n\
 * xhtml11 specific\n\
 *\n\
 * */\n\
\n\
tt {\n\
  font-family: monospace;\n\
  font-size: inherit;\n\
  color: navy;\n\
}\n\
\n\
div.tableblock {\n\
  margin-top: 1.0em;\n\
  margin-bottom: 1.5em;\n\
}\n\
div.tableblock > table {\n\
  border: 3px solid #527bbd;\n\
}\n\
thead, p.table.header {\n\
  font-weight: bold;\n\
  color: #527bbd;\n\
}\n\
p.table {\n\
  margin-top: 0;\n\
}\n\
/* Because the table frame attribute is overriden by CSS in most browsers. */\n\
div.tableblock > table[frame=\"void\"] {\n\
  border-style: none;\n\
}\n\
div.tableblock > table[frame=\"hsides\"] {\n\
  border-left-style: none;\n\
  border-right-style: none;\n\
}\n\
div.tableblock > table[frame=\"vsides\"] {\n\
  border-top-style: none;\n\
  border-bottom-style: none;\n\
}\n\
\n\
\n\
/*\n\
 * html5 specific\n\
 *\n\
 * */\n\
\n\
.monospaced {\n\
  font-family: monospace;\n\
  font-size: inherit;\n\
  color: navy;\n\
}\n\
\n\
table.tableblock {\n\
  margin-top: 1.0em;\n\
  margin-bottom: 1.5em;\n\
}\n\
thead, p.tableblock.header {\n\
  font-weight: bold;\n\
  color: #527bbd;\n\
}\n\
p.tableblock {\n\
  margin-top: 0;\n\
}\n\
table.tableblock {\n\
  border-width: 3px;\n\
  border-spacing: 0px;\n\
  border-style: solid;\n\
  border-color: #527bbd;\n\
  border-collapse: collapse;\n\
}\n\
th.tableblock, td.tableblock {\n\
  border-width: 1px;\n\
  padding: 4px;\n\
  border-style: solid;\n\
  border-color: #527bbd;\n\
}\n\
\n\
table.tableblock.frame-topbot {\n\
  border-left-style: hidden;\n\
  border-right-style: hidden;\n\
}\n\
table.tableblock.frame-sides {\n\
  border-top-style: hidden;\n\
  border-bottom-style: hidden;\n\
}\n\
table.tableblock.frame-none {\n\
  border-style: hidden;\n\
}\n\
\n\
th.tableblock.halign-left, td.tableblock.halign-left {\n\
  text-align: left;\n\
}\n\
th.tableblock.halign-center, td.tableblock.halign-center {\n\
  text-align: center;\n\
}\n\
th.tableblock.halign-right, td.tableblock.halign-right {\n\
  text-align: right;\n\
}\n\
\n\
th.tableblock.valign-top, td.tableblock.valign-top {\n\
  vertical-align: top;\n\
}\n\
th.tableblock.valign-middle, td.tableblock.valign-middle {\n\
  vertical-align: middle;\n\
}\n\
th.tableblock.valign-bottom, td.tableblock.valign-bottom {\n\
  vertical-align: bottom;\n\
}\n\
\n\
\n\
/*\n\
 * manpage specific\n\
 *\n\
 * */\n\
\n\
body.manpage h1 {\n\
  padding-top: 0.5em;\n\
  padding-bottom: 0.5em;\n\
  border-top: 2px solid silver;\n\
  border-bottom: 2px solid silver;\n\
}\n\
body.manpage h2 {\n\
  border-style: none;\n\
}\n\
body.manpage div.sectionbody {\n\
  margin-left: 3em;\n\
}\n\
\n\
@media print {\n\
  body.manpage div#toc { display: none; }\n\
}\n\
</style>\n\
<script type=\"text/javascript\">\n\
/*<![CDATA[*/\n\
var asciidoc = {  // Namespace.\n\
\n\
/////////////////////////////////////////////////////////////////////\n\
// Table Of Contents generator\n\
/////////////////////////////////////////////////////////////////////\n\
\n\
/* Author: Mihai Bazon, September 2002\n\
 * http://students.infoiasi.ro/~mishoo\n\
 *\n\
 * Table Of Content generator\n\
 * Version: 0.4\n\
 *\n\
 * Feel free to use this script under the terms of the GNU General Public\n\
 * License, as long as you do not remove or alter this notice.\n\
 */\n\
\n\
 /* modified by Troy D. Hanson, September 2006. License: GPL */\n\
 /* modified by Stuart Rackham, 2006, 2009. License: GPL */\n\
\n\
// toclevels = 1..4.\n\
toc: function (toclevels) {\n\
\n\
  function getText(el) {\n\
    var text = \"\";\n\
    for (var i = el.firstChild; i != null; i = i.nextSibling) {\n\
      if (i.nodeType == 3 /* Node.TEXT_NODE */) // IE doesn't speak constants.\n\
        text += i.data;\n\
      else if (i.firstChild != null)\n\
        text += getText(i);\n\
    }\n\
    return text;\n\
  }\n\
\n\
  function TocEntry(el, text, toclevel) {\n\
    this.element = el;\n\
    this.text = text;\n\
    this.toclevel = toclevel;\n\
  }\n\
\n\
  function tocEntries(el, toclevels) {\n\
    var result = new Array;\n\
    var re = new RegExp('[hH]([2-'+(toclevels+1)+'])');\n\
    // Function that scans the DOM tree for header elements (the DOM2\n\
    // nodeIterator API would be a better technique but not supported by all\n\
    // browsers).\n\
    var iterate = function (el) {\n\
      for (var i = el.firstChild; i != null; i = i.nextSibling) {\n\
        if (i.nodeType == 1 /* Node.ELEMENT_NODE */) {\n\
          var mo = re.exec(i.tagName);\n\
          if (mo && (i.getAttribute(\"class\") || i.getAttribute(\"className\")) != \"float\") {\n\
            result[result.length] = new TocEntry(i, getText(i), mo[1]-1);\n\
          }\n\
          iterate(i);\n\
        }\n\
      }\n\
    }\n\
    iterate(el);\n\
    return result;\n\
  }\n\
\n\
  var toc = document.getElementById(\"toc\");\n\
  if (!toc) {\n\
    return;\n\
  }\n\
\n\
  // Delete existing TOC entries in case we're reloading the TOC.\n\
  var tocEntriesToRemove = [];\n\
  var i;\n\
  for (i = 0; i < toc.childNodes.length; i++) {\n\
    var entry = toc.childNodes[i];\n\
    if (entry.nodeName == 'div'\n\
     && entry.getAttribute(\"class\")\n\
     && entry.getAttribute(\"class\").match(/^toclevel/))\n\
      tocEntriesToRemove.push(entry);\n\
  }\n\
  for (i = 0; i < tocEntriesToRemove.length; i++) {\n\
    toc.removeChild(tocEntriesToRemove[i]);\n\
  }\n\
\n\
  // Rebuild TOC entries.\n\
  var entries = tocEntries(document.getElementById(\"content\"), toclevels);\n\
  for (var i = 0; i < entries.length; ++i) {\n\
    var entry = entries[i];\n\
    if (entry.element.id == \"\")\n\
      entry.element.id = \"_toc_\" + i;\n\
    var a = document.createElement(\"a\");\n\
    a.href = \"#\" + entry.element.id;\n\
    a.appendChild(document.createTextNode(entry.text));\n\
    var div = document.createElement(\"div\");\n\
    div.appendChild(a);\n\
    div.className = \"toclevel\" + entry.toclevel;\n\
    toc.appendChild(div);\n\
  }\n\
  if (entries.length == 0)\n\
    toc.parentNode.removeChild(toc);\n\
},\n\
\n\
\n\
/////////////////////////////////////////////////////////////////////\n\
// Footnotes generator\n\
/////////////////////////////////////////////////////////////////////\n\
\n\
/* Based on footnote generation code from:\n\
 * http://www.brandspankingnew.net/archive/2005/07/format_footnote.html\n\
 */\n\
\n\
footnotes: function () {\n\
  // Delete existing footnote entries in case we're reloading the footnodes.\n\
  var i;\n\
  var noteholder = document.getElementById(\"footnotes\");\n\
  if (!noteholder) {\n\
    return;\n\
  }\n\
  var entriesToRemove = [];\n\
  for (i = 0; i < noteholder.childNodes.length; i++) {\n\
    var entry = noteholder.childNodes[i];\n\
    if (entry.nodeName == 'div' && entry.getAttribute(\"class\") == \"footnote\")\n\
      entriesToRemove.push(entry);\n\
  }\n\
  for (i = 0; i < entriesToRemove.length; i++) {\n\
    noteholder.removeChild(entriesToRemove[i]);\n\
  }\n\
\n\
  // Rebuild footnote entries.\n\
  var cont = document.getElementById(\"content\");\n\
  var spans = cont.getElementsByTagName(\"span\");\n\
  var refs = {};\n\
  var n = 0;\n\
  for (i=0; i<spans.length; i++) {\n\
    if (spans[i].className == \"footnote\") {\n\
      n++;\n\
      var note = spans[i].getAttribute(\"data-note\");\n\
      if (!note) {\n\
        // Use [\\s\\S] in place of . so multi-line matches work.\n\
        // Because JavaScript has no s (dotall) regex flag.\n\
        note = spans[i].innerHTML.match(/\\s*\[([\\s\\S]*)]\\s*/)[1];\n\
        spans[i].innerHTML =\n\
          \"[<a id='_footnoteref_\" + n + \"' href='#_footnote_\" + n +\n\
          \"' title='View footnote' class='footnote'>\" + n + \"</a>]\";\n\
        spans[i].setAttribute(\"data-note\", note);\n\
      }\n\
      noteholder.innerHTML +=\n\
        \"<div class='footnote' id='_footnote_\" + n + \"'>\" +\n\
        \"<a href='#_footnoteref_\" + n + \"' title='Return to text'>\" +\n\
        n + \"</a>. \" + note + \"</div>\";\n\
      var id =spans[i].getAttribute(\"id\");\n\
      if (id != null) refs[\"#\"+id] = n;\n\
    }\n\
  }\n\
  if (n == 0)\n\
    noteholder.parentNode.removeChild(noteholder);\n\
  else {\n\
    // Process footnoterefs.\n\
    for (i=0; i<spans.length; i++) {\n\
      if (spans[i].className == \"footnoteref\") {\n\
        var href = spans[i].getElementsByTagName(\"a\")[0].getAttribute(\"href\");\n\
        href = href.match(/#.*/)[0];  // Because IE return full URL.\n\
        n = refs[href];\n\
        spans[i].innerHTML =\n\
          \"[<a href='#_footnote_\" + n +\n\
          \"' title='View footnote' class='footnote'>\" + n + \"</a>]\";\n\
      }\n\
    }\n\
  }\n\
},\n\
\n\
install: function(toclevels) {\n\
  var timerId;\n\
\n\
  function reinstall() {\n\
    asciidoc.footnotes();\n\
    if (toclevels) {\n\
      asciidoc.toc(toclevels);\n\
    }\n\
  }\n\
\n\
  function reinstallAndRemoveTimer() {\n\
    clearInterval(timerId);\n\
    reinstall();\n\
  }\n\
\n\
  timerId = setInterval(reinstall, 500);\n\
  if (document.addEventListener)\n\
    document.addEventListener(\"DOMContentLoaded\", reinstallAndRemoveTimer, false);\n\
  else\n\
    window.onload = reinstallAndRemoveTimer;\n\
}\n\
\n\
}\n\
asciidoc.install();\n\
/*]]>*/\n\
</script>\n\
</head>\n\
<body class=\"article\">\n\
<div id=\"header\">\n\
<h1>Guide to linsim</h1>\n\
</div>\n\
<div id=\"content\">\n\
<div id=\"preamble\">\n\
<div class=\"sectionbody\">\n\
<div class=\"sidebarblock\">\n\
<div class=\"content\">\n\
<div class=\"paragraph\"><p>Of necessity, this Guide contains only as much as you need to know to get started.</p></div>\n\
</div></div>\n\
</div>\n\
</div>\n\
<div class=\"sect1\">\n\
<h2 id=\"ref-general\">1. What is Linsim?</h2>\n\
<div class=\"sectionbody\">\n\
<div class=\"paragraph\"><p>linsim is a computer program intended as a tool for Amateur Radio Digital Mode\n\
evaluation under varying HF propogation conditions.</p></div>\n\
<div class=\"paragraph\"><p>linsim is available for multiple computer operating systems;\n\
FreeBSD&#8482;; Linux&#8482;, OS X&#8482; and Windows&#8482;.</p></div>\n\
<div class=\"sect2\">\n\
<h3 id=\"_what_is_a_simulation_model\">1.1. What is a Simulation Model?</h3>\n\
<div class=\"paragraph\"><div class=\"title\">Watterson Ionospheric Channel Model</div><p>Radio propagation via the earth&#8217;s ionosphere contain reflections from 3 general effects:</p></div>\n\
<div class=\"ulist\"><ul>\n\
<li>\n\
<p>\n\
Density variations versus altitude in the ionosphere\n\
</p>\n\
</li>\n\
<li>\n\
<p>\n\
Magnetic-ionic effects that cause polarization-dependent paths\n\
</p>\n\
</li>\n\
<li>\n\
<p>\n\
Non-uniformity of the ionospheric layers\n\
</p>\n\
</li>\n\
</ul></div>\n\
<div class=\"paragraph\"><p>The Watterson HF channel model has been used for some time for simulating the propagation of radio waves reflecting off of the ionosphere by simulating both amplitude and phase distortion in multiple propagation paths each with a specified fixed delay.</p></div>\n\
<div class=\"paragraph\"><p>For a much more detailed description, one is referred to an article in the May/June 2000 issue of QEX by Johan Forrer describing the details of such a model. Another reference can be found in \"Wireless Digital Communications:\n\
Design and Theory\" by Tom McDermott.</p></div>\n\
<div class=\"paragraph\"><p>The mathematics associated with the various signal processing paths are identical to PathSim.  The reader is refered to Moe Wheatley&#8217;s excellent PathSim technical document for details, <a href=\"http://www.moetronix.com/ae4jy/files/pathsimtech100.pdf\">http://www.moetronix.com/ae4jy/files/pathsimtech100.pdf</a>.</p></div>\n\
</div>\n\
<div class=\"sect2\">\n\
<h3 id=\"_general_description\">1.2. General description</h3>\n\
<div class=\"paragraph\"><p>Linsim is designed to read and then add path simulation to any monophonic wav file recorded at any sampling rate.  It works particularly well with files that were created using fldigi&#8217;s audio capture and audio generate functions.  The entire wav file will be saved to computer memory and then duplicated during the signal processing.  The user should try to keep the length of the wav file at 20 Mg or less, but the author has tested some 200 Mg files on both Linux and Windows-8 without causing a program fault.  These files were original VOAR broadcasts of about 30 minutes duration.  The objective of this type of simulation is to finally measure the character error rate (CER) and bit error rate (BER) of a specific modem type and decoder design.  For most modems a sequence of 1000 characters provides a sufficient level of confidence in the CER measurment.</p></div>\n\
<div class=\"paragraph\"><p>Simulations include path delay, doppler, frequency shift and band limited Gaussian white noise.  All simulations are constrained to a 3000 Hz bandwidth.  The input signal is filtered by a FIR bandpass filter (400 - 3400 Hz).</p></div>\n\
<div class=\"admonitionblock\">\n\
<table><tr>\n\
<td class=\"icon\">\n\
<div class=\"title\">Note</div>\n\
</td>\n\
<td class=\"content\">\n\
<div class=\"paragraph\"><p>Your input signal should not extend beyond these limits.</p></div>\n\
</td>\n\
</tr></table>\n\
</div>\n\
</div>\n\
</div>\n\
</div>\n\
<div class=\"sect1\">\n\
<h2 id=\"ref-folders-files\">2. Simulation Specification Files</h2>\n\
<div class=\"sectionbody\">\n\
<div class=\"sect2\">\n\
<h3 id=\"_folder_location\">2.1. Folder location</h3>\n\
<div class=\"paragraph\"><p>The first time it is executed linsim will create a folder in which to store it&#8217;s simulation specification files.  The folder location depends on the user&#8217;s operating system:</p></div>\n\
<div class=\"paragraph\"><p>XP : C:\\Documents and Settings\\&lt;user&gt;\\linsim.files</p></div>\n\
<div class=\"paragraph\"><p>Vista / Win7 / Win8 : C:\\Users\\&lt;user&gt;\\linsim.files</p></div>\n\
<div class=\"paragraph\"><p>Linux : /home/&lt;user&gt;/.linsim</p></div>\n\
<div class=\"paragraph\"><p>OS X : /users/&lt;user&gt;/.linsim</p></div>\n\
<div class=\"paragraph\"><p>Linux and OS X users might want to create a link to that folder</p></div>\n\
<div class=\"paragraph\"><p>$ cd\n\
$ ln -s linsim.files .linsim</p></div>\n\
</div>\n\
</div>\n\
</div>\n\
<div class=\"sect1\">\n\
<h2 id=\"_simulation_files\">3. Simulation Files</h2>\n\
<div class=\"sectionbody\">\n\
<div class=\"paragraph\"><p>Linsim can manage, open and save any number of simulation sets.  Each set of simulations are saved in the same comma-separated-value (csv) file.  The files are compatible with any spreadsheet program such as Excel, Open Office, or Gnumeric.</p></div>\n\
<div class=\"sect2\">\n\
<h3 id=\"_simulation_file_format\">3.1. Simulation File Format</h3>\n\
<div class=\"paragraph\"><p>Simulation files are spreadsheet files saved in a comma-separated-value format.\n\
A default set of simulations are created for you when the program is executed\n\
for the first time.  The default file contents are</p></div>\n\
<div class=\"tableblock\">\n\
<table rules=\"all\"\n\
width=\"100%\"\n\
frame=\"border\"\n\
cellspacing=\"0\" cellpadding=\"4\">\n\
<col width=\"3%\" />\n\
<col width=\"6%\" />\n\
<col width=\"6%\" />\n\
<col width=\"6%\" />\n\
<col width=\"6%\" />\n\
<col width=\"6%\" />\n\
<col width=\"6%\" />\n\
<col width=\"6%\" />\n\
<col width=\"6%\" />\n\
<col width=\"6%\" />\n\
<col width=\"6%\" />\n\
<col width=\"6%\" />\n\
<col width=\"6%\" />\n\
<col width=\"6%\" />\n\
<col width=\"6%\" />\n\
<thead>\n\
<tr>\n\
<th align=\"center\" valign=\"top\">TITLE</th>\n\
<th align=\"left\" valign=\"top\">AWGN</th>\n\
<th align=\"left\" valign=\"top\">S/N</th>\n\
<th align=\"left\" valign=\"top\">P1</th>\n\
<th align=\"left\" valign=\"top\">SPREAD_1</th>\n\
<th align=\"left\" valign=\"top\">OFFSET_1</th>\n\
<th align=\"left\" valign=\"top\">P2</th>\n\
<th align=\"left\" valign=\"top\">DELAY_2</th>\n\
<th align=\"left\" valign=\"top\">SPREAD_2</th>\n\
<th align=\"left\" valign=\"top\">OFFSET_2</th>\n\
<th align=\"left\" valign=\"top\">P3</th>\n\
<th align=\"left\" valign=\"top\">DELAY_3</th>\n\
<th align=\"left\" valign=\"top\">SPREAD_3</th>\n\
<th align=\"left\" valign=\"top\">OFFSET_3</th>\n\
</tr>\n\
</thead>\n\
<tbody>\n\
<tr>\n\
<td align=\"center\" valign=\"top\"><p class=\"table\">AWGN S/N</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">1</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">10</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
</tr>\n\
<tr>\n\
<td align=\"center\" valign=\"top\"><p class=\"table\">CCIR 520-2 (Doppler Fading)</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">1</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0.2</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">10</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">1</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0.5</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0.2</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">5</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
</tr>\n\
<tr>\n\
<td align=\"center\" valign=\"top\"><p class=\"table\">CCIR 520-2 (Flutter Fading)</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">1</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">10</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">1</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0.5</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">10</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
</tr>\n\
<tr>\n\
<td align=\"center\" valign=\"top\"><p class=\"table\">CCIR 520-2 (Good Conditions)</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">1</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0.1</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">1</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0.5</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0.1</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
</tr>\n\
<tr>\n\
<td align=\"center\" valign=\"top\"><p class=\"table\">CCIR-520-2 (Moderate Conditions)</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">1</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0.5</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">1</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">1</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0.5</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
</tr>\n\
<tr>\n\
<td align=\"center\" valign=\"top\"><p class=\"table\">CCIR-520-2 (Poor Conditions)</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">1</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0.5</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">1</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">2</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0.5</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
</tr>\n\
<tr>\n\
<td align=\"center\" valign=\"top\"><p class=\"table\">Direct Path</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">10</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
</tr>\n\
<tr>\n\
<td align=\"center\" valign=\"top\"><p class=\"table\">Frequency Shifter</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">1</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">500</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
</tr>\n\
<tr>\n\
<td align=\"center\" valign=\"top\"><p class=\"table\">High-Latitude Disturbed</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">1</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">30</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">1</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">7</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">30</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
</tr>\n\
<tr>\n\
<td align=\"center\" valign=\"top\"><p class=\"table\">High-Latitude Moderate</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">1</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">10</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">1</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">3</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">10</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
</tr>\n\
<tr>\n\
<td align=\"center\" valign=\"top\"><p class=\"table\">High-Latitude Quiet</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">1</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0.5</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">1</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">1</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0.5</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
</tr>\n\
<tr>\n\
<td align=\"center\" valign=\"top\"><p class=\"table\">Low-Latitude Disturbed</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">1</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">10</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">1</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">6</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">10</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
</tr>\n\
<tr>\n\
<td align=\"center\" valign=\"top\"><p class=\"table\">Low-Latitude Moderate</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">1</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">1.5</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">1</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">2</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">1.5</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
</tr>\n\
<tr>\n\
<td align=\"center\" valign=\"top\"><p class=\"table\">Low-Latitude Quiet</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">1</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0.5</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">1</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0.5</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0.5</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
</tr>\n\
<tr>\n\
<td align=\"center\" valign=\"top\"><p class=\"table\">Mid-Latitude Disturbed</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">1</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">1</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">1</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">2</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">1</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
</tr>\n\
<tr>\n\
<td align=\"center\" valign=\"top\"><p class=\"table\">Mid-Latitude Disturbed NVIS</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">1</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">1</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">1</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">7</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">1</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
</tr>\n\
<tr>\n\
<td align=\"center\" valign=\"top\"><p class=\"table\">Mid-Latitude Moderate</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">1</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0.5</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">1</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">1</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0.5</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
</tr>\n\
<tr>\n\
<td align=\"center\" valign=\"top\"><p class=\"table\">Mid-Latitude Quiet</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">1</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0.5</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">1</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0.5</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0.5</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
<td align=\"left\" valign=\"top\"><p class=\"table\">0</p></td>\n\
</tr>\n\
</tbody>\n\
</table>\n\
</div>\n\
</div>\n\
</div>\n\
</div>\n\
<div class=\"sect1\">\n\
<h2 id=\"ref-operating\">4. Menu functions</h2>\n\
<div class=\"sectionbody\">\n\
<div class=\"ulist\"><div class=\"title\">File</div><ul>\n\
<li>\n\
<p>\n\
Open    - open an existing simulation set csv file\n\
</p>\n\
<div class=\"ulist\"><ul>\n\
<li>\n\
<p>\n\
the name of the simulation file in use will be displayed in the control labeled \"Models:\"\n\
</p>\n\
</li>\n\
</ul></div>\n\
</li>\n\
<li>\n\
<p>\n\
Save    - save the current simulation set csv file\n\
</p>\n\
</li>\n\
<li>\n\
<p>\n\
Save As - save the current simulation set, specifying a new file path-name\n\
</p>\n\
</li>\n\
<li>\n\
<p>\n\
Exit    - exit the program, saving current simulation set\n\
</p>\n\
</li>\n\
</ul></div>\n\
<div class=\"ulist\"><div class=\"title\">Simulation</div><ul>\n\
<li>\n\
<p>\n\
Add         - add the current simulation specifications to the current set\n\
</p>\n\
</li>\n\
<li>\n\
<p>\n\
Batch       - open the batch processing dialog\n\
</p>\n\
<div class=\"ulist\"><ul>\n\
<li>\n\
<p>\n\
select sequence of simulations to be run as a set\n\
</p>\n\
</li>\n\
</ul></div>\n\
</li>\n\
<li>\n\
<p>\n\
AWGN series - open the AWGN series processing dialog\n\
</p>\n\
<div class=\"ulist\"><ul>\n\
<li>\n\
<p>\n\
select lower, upper, step for a series of simulation runs\n\
</p>\n\
</li>\n\
</ul></div>\n\
</li>\n\
<li>\n\
<p>\n\
Choose output folder\n\
</p>\n\
<div class=\"ulist\"><ul>\n\
<li>\n\
<p>\n\
open folder (directory) selection folder\n\
</p>\n\
</li>\n\
<li>\n\
<p>\n\
select target folder (must be existing)\n\
</p>\n\
</li>\n\
<li>\n\
<p>\n\
or output same as input folder (default)\n\
</p>\n\
</li>\n\
</ul></div>\n\
</li>\n\
</ul></div>\n\
<div class=\"ulist\"><div class=\"title\">Samplerate - select from one of several menu choices</div><ul>\n\
<li>\n\
<p>\n\
Same as input (default)\n\
</p>\n\
</li>\n\
<li>\n\
<p>\n\
8000\n\
</p>\n\
</li>\n\
<li>\n\
<p>\n\
11025\n\
</p>\n\
</li>\n\
<li>\n\
<p>\n\
16000\n\
</p>\n\
</li>\n\
<li>\n\
<p>\n\
22050\n\
</p>\n\
</li>\n\
<li>\n\
<p>\n\
24000\n\
</p>\n\
</li>\n\
<li>\n\
<p>\n\
44100\n\
</p>\n\
</li>\n\
<li>\n\
<p>\n\
48000\n\
</p>\n\
</li>\n\
</ul></div>\n\
<div class=\"ulist\"><div class=\"title\">Help</div><ul>\n\
<li>\n\
<p>\n\
About - Version, authors, etc.\n\
</p>\n\
</li>\n\
<li>\n\
<p>\n\
Guide - open this html document in default html browser\n\
</p>\n\
</li>\n\
</ul></div>\n\
</div>\n\
</div>\n\
<div class=\"sect1\">\n\
<h2 id=\"_main_dialog_controls\">5. Main Dialog Controls</h2>\n\
<div class=\"sectionbody\">\n\
<div class=\"ulist\"><ul>\n\
<li>\n\
<p>\n\
Input button - select the input file.\n\
</p>\n\
<div class=\"ulist\"><ul>\n\
<li>\n\
<p>\n\
selection will be shown in the output control to the right of the button.\n\
</p>\n\
</li>\n\
</ul></div>\n\
</li>\n\
<li>\n\
<p>\n\
Output button - select the output file.\n\
</p>\n\
<div class=\"ulist\"><ul>\n\
<li>\n\
<p>\n\
selection will be shown in the output control to the right of the button.\n\
</p>\n\
</li>\n\
</ul></div>\n\
</li>\n\
<li>\n\
<p>\n\
Simul' button - open the simulation select dialog\n\
</p>\n\
<div class=\"ulist\"><ul>\n\
<li>\n\
<p>\n\
The select dialog contains 5 controls\n\
</p>\n\
<div class=\"ulist\"><ul>\n\
<li>\n\
<p>\n\
simulation browser - select the desired simulation with left mouse button\n\
</p>\n\
</li>\n\
<li>\n\
<p>\n\
Select button - load the constraints for the highlighted simulation to the main dialog\n\
</p>\n\
<div class=\"ulist\"><ul>\n\
<li>\n\
<p>\n\
Once selected the next two controls become accessible\n\
</p>\n\
</li>\n\
<li>\n\
<p>\n\
Make any desired changes to the constraint values on the main dialog\n\
</p>\n\
</li>\n\
</ul></div>\n\
</li>\n\
<li>\n\
<p>\n\
Update button - update the highlighted simulation with the main dialog constraints\n\
</p>\n\
</li>\n\
<li>\n\
<p>\n\
Delete button - delete the current simulation from the list\n\
</p>\n\
</li>\n\
<li>\n\
<p>\n\
Cancel button - close the select dialog\n\
</p>\n\
</li>\n\
</ul></div>\n\
</li>\n\
</ul></div>\n\
</li>\n\
</ul></div>\n\
<div class=\"admonitionblock\">\n\
<table><tr>\n\
<td class=\"icon\">\n\
<div class=\"title\">Note</div>\n\
</td>\n\
<td class=\"content\">\n\
<div class=\"title\">Adding a new simulation</div>\n\
<div class=\"paragraph\"><p>You may also modify the constraints, change the simulation name accordingly, and then use the Simulation, Add menu item to add the new simulation to the displayed list.</p></div>\n\
</td>\n\
</tr></table>\n\
</div>\n\
</div>\n\
</div>\n\
<div class=\"sect1\">\n\
<h2 id=\"_path_specifications\">6. Path specifications</h2>\n\
<div class=\"sectionbody\">\n\
<div class=\"paragraph\"><p>There are 3 possible path specifications that can be applied concurrently to the input audio stream.  Path 1 is the ground wave or direct signal path and is always present in the output signal.  Path 2 and 3 are delayed signal paths, usually sky wave path simulation.  Paths 2 and 3 can be either selected or not by the check box associated with each path.</p></div>\n\
<div class=\"ulist\"><ul>\n\
<li>\n\
<p>\n\
Path 1 - undelayed, straight line, or ground wave path; always present\n\
</p>\n\
<div class=\"ulist\"><ul>\n\
<li>\n\
<p>\n\
Spread - 2 sigma Doppler spread in Hz\n\
</p>\n\
</li>\n\
<li>\n\
<p>\n\
Offset - Doppler shift in Hz\n\
</p>\n\
</li>\n\
</ul></div>\n\
</li>\n\
<li>\n\
<p>\n\
Path 2 - reflected signal path, usually skywave\n\
</p>\n\
<div class=\"ulist\"><ul>\n\
<li>\n\
<p>\n\
Delay - path delay from ground wave path, in milliseconds\n\
</p>\n\
</li>\n\
<li>\n\
<p>\n\
Spread - 2 sigma Doppler spread in Hz\n\
</p>\n\
</li>\n\
<li>\n\
<p>\n\
Offset - Doppler shift in Hz\n\
</p>\n\
</li>\n\
</ul></div>\n\
</li>\n\
<li>\n\
<p>\n\
Path 3 - reflected signal path, usually skywave\n\
</p>\n\
<div class=\"ulist\"><ul>\n\
<li>\n\
<p>\n\
Delay - path delay from ground wave path, in milliseconds\n\
</p>\n\
</li>\n\
<li>\n\
<p>\n\
Spread - 2 sigma Doppler spread in Hz\n\
</p>\n\
</li>\n\
<li>\n\
<p>\n\
Offset - Doppler shift in Hz\n\
</p>\n\
</li>\n\
</ul></div>\n\
</li>\n\
</ul></div>\n\
</div>\n\
</div>\n\
<div class=\"sect1\">\n\
<h2 id=\"_awgn_specifications\">7. AWGN specifications</h2>\n\
<div class=\"sectionbody\">\n\
<div class=\"paragraph\"><p>Gaussian noise can be added to the signal.  The S/N value is in dB with respect to the input audio stream; ie: 6 dB results in an output audio stream with a +6 dB signal to noise ratio.</p></div>\n\
</div>\n\
</div>\n\
<div class=\"sect1\">\n\
<h2 id=\"ref-batch\">8. Batch Processing</h2>\n\
<div class=\"sectionbody\">\n\
<div class=\"paragraph\"><p>The batch processing dialog is similar to the select dialog in that it contains a list browser and 4 buttons.  The list browser is different in that each line can be selected by toggling it&#8217;s associated check box.</p></div>\n\
<div class=\"ulist\"><ul>\n\
<li>\n\
<p>\n\
Clear button   - clear all list check boxes\n\
</p>\n\
</li>\n\
<li>\n\
<p>\n\
All button     - set all list check boxes\n\
</p>\n\
</li>\n\
<li>\n\
<p>\n\
Process button - process each of the checked list items\n\
</p>\n\
<div class=\"ulist\"><ul>\n\
<li>\n\
<p>\n\
as each item is processed a simulation will be performed and the output audio stream written to a file whose name is a composite of the input audio stream file-name and the current simulation name.  The main dialog constraints will display the simulation values as each one is processed.  White space and directory/folder designation characters will be change to an underscore for the final file-name.  The file will be either stored in the same folder as the input wav file or in the designated output file folder.\n\
</p>\n\
</li>\n\
</ul></div>\n\
</li>\n\
<li>\n\
<p>\n\
Cancel         - close the batch process dialog\n\
</p>\n\
</li>\n\
</ul></div>\n\
</div>\n\
</div>\n\
<div class=\"sect1\">\n\
<h2 id=\"ref-series\">9. AWGN Series Processing</h2>\n\
<div class=\"sectionbody\">\n\
<div class=\"paragraph\"><p>It&#8217;s quite common to want a series of test files at varying signal to noise ratios.  The menu item \"Simulation | AWGN series\" gives access to a simple dialog that allows the user to specify the lower, upper and step size for a sequence of AWGN tests.  Set these values and then press the \"Process\" button to generate a unique output wav file for each of requested series.  The output file-name will be a composite of the input file name and the AWGN value of interest.  Output folder designation is the same as for the batch processing.</p></div>\n\
</div>\n\
</div>\n\
<div class=\"sect1\">\n\
<h2 id=\"ref-credits\">10. Credits</h2>\n\
<div class=\"sectionbody\">\n\
<div class=\"paragraph\"><p>Copyright &#169; 2014 David Freese, <tt>W1HKJ</tt>.</p></div>\n\
<div class=\"paragraph\"><p>Based on AE4JY, Moe Wheatley&#8217;s PathSim.</p></div>\n\
<div class=\"paragraph\"><p>License GPLv3+: <a href=\"http://www.gnu.org/licenses/gpl.html\">GNU GPL version 3 or later</a>.</p></div>\n\
</div>\n\
</div>\n\
</div>\n\
<div id=\"footnotes\"><hr /></div>\n\
<div id=\"footer\">\n\
<div id=\"footer-text\">\n\
Last updated 2014-01-08 14:47:46 CST\n\
</div>\n\
</div>\n\
</body>\n\
</html>";
