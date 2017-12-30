# Artwork look and effects
Skyscraper allows you to fully customize how you want the final frontend artwork to appear. Each piece of artwork can be composited of one or more of the other artwork resources and you can even apply effects to them. Read on for a full description on how to do this.

## Where all the magic happens
All artwork rendering is currently handled by the xml file located at '`~/.skyscraper/artwork.xml`'. An example could look something like this:
```
<?xml version="1.0" encoding="UTF-8"?>
<artwork>
  <!--
      <output>
      - Leave out 'width' and 'height' to use original dimensions of the type resource
      - If an output node contains no image nodes, it will save it exactly as it was delivered by the scraping source
  -->
  <output type="screenshot" width="640" height="400">
    <!--
	<image>
	- Leave out 'width' or 'height' to keep aspect ratio.
	- Leave out both 'width' and 'height' to use original size.
	- 'align' can be 'left', 'center' or 'right'
	- 'valign' can be 'top', 'middle' or 'bottom'
	- 'x' and 'y' defines top-left coordinate. It is relative to 'align' and 'valign' if used.
	- Image nodes are rendered from bottom to top.
    -->
    <image resource="wheel" x="5" y="5" width="10" height="10"/>
    <image resource="cover" valign="bottom" y="-15" height="200">
      <shadow distance="10" softness="10" opacity="75"/>
    </image>
    <image resource="screenshot" align="right" valign="top" width="520" height="390"/>
  </output>
  <!-- This will save the 'cover' without altering it, keeping its originla dimensions. -->
  <output type="cover"/>
  <!-- This will save the 'wheel' and resize it to width, keeping the aspect ratio -->
  <output type="wheel" width="100"/>
  <!-- This will save the 'marquee' and resize it, ignoring the aspect ratio -->
  <output type="marquee" width="100" height="100"/>
</artwork>
```

### <?xml?> node
```
<?xml version="1.0" encoding="UTF-8"?>
```
This is a special node used to describe the xml language used in the document.

### <artwork> node
This node is the document base node. It contains all other nodes.

### <output> node

### <image> node
#### <shadow> node

![Small cover with drop shadow, larger screenshot](https://raw.githubusercontent.com/muldjord/skyscraper/master/artwork_examples/Bubble%20Bobble.png)
