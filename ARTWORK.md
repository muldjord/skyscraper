# Artwork look and effects
Skyscraper allows you to fully customize how you want the final frontend artwork to appear. Each piece of artwork can be composited of one or more of the other artwork resources and you can even apply effects to them. Read on for a full description on how to do this.

NOTE!!! The documentation is currently being updated with the new features in the yet unreleased version 2.3.0. Some of the effects below will not work until release (which is very soon).

## Where all the magic happens
All artwork rendering is handled by the xml file located at '`~/.skyscraper/artwork.xml`'. An example could look something like this:
```
<?xml version="1.0" encoding="UTF-8"?>
<artwork>
  <output type="screenshot" width="640" height="480">
    <layer resource="screenshot" x="20" width="520" height="390" align="center" valign="middle">
      <rounded radius="10"/>
      <stroke width="5"/>
    </layer>
    <layer resource="cover" height="250" x="0" y="-10" valign="bottom">
      <gamebox side="wheel" rotate="90"/>
      <shadow distance="5" softness="5" opacity="70"/>
    </layer>
    <layer resource="wheel" width="250" x="-10" align="right">
      <shadow distance="5" softness="5" opacity="70"/>
    </layer>
  </output>
</artwork>
```
Which results in this:

![Small cover with drop shadow, larger screenshot](https://raw.githubusercontent.com/muldjord/skyscraper/master/artwork_examples/Sonic%20The%20Hedgehog%202.png)

## Xml definitions
Read on for a full description of the xml format used in 'artwork.xml'

### xml node
```
<?xml version="1.0" encoding="UTF-8"?>
```
This is a special node used to describe the xml language used in the document.

### 'artwork' node
```
<artwork>
...
</artwork>
```
This node is the document base node. It contains all other nodes.

### 'output' node(s) (Optional)
```
<output type="screenshot" width="640" height="400">
...
<output>
```
The output node describes a single piece of artwork to be exported when scraping. You can have as many (or little) output nodes as you like.

An output node with no nested 'layer' nodes will export the type with no compositing.

#### 'type' attribute
The type to be exported. Can be:
* screenshot
* cover
* wheel
* marquee

#### 'width' and 'height' attributes
Sets the width and height of the artwork export. If both are left out it will use the original size of the scraped artwork. If one is left out, it will keep the aspect ratio relative to that.

### 'layer' node(s) (Optional)
```
<layer resource="cover" x="5" y="5" width="10" height="10" align="center" valign="middle"/>
...
</layer>
```
Like layers in Gimp or almost any other respectable photo manipulation software, you can add as many of these nodes as you like. They will be rendered from bottom to top.

#### 'resource' attribute (Not optional)
The artwork resource to be composited. Can be:
* screenshot
* cover
* wheel
* marquee

#### 'x' and 'y' attributes (Optional)
X and Y coordinates for placement. Relative to 'align' and 'valign'. Either can be left out in which case it will be set to 0.

#### 'width' and 'height' attributes (Optional)
Sets the width and height of the layer. If either is left out it will keep the aspect ratio relative to the other. If both are left out it will use the original dimensions of the scraped resource.

#### 'align' attribute (Optional)
The horizontal alignment of the layer. Can be:
* left
* center
* right

#### 'valign' attribute (Optional)
The vertical alignment of the layer. Can be:
* top
* middle
* bottom

### 'shadow' node (Optional)
```
<layer>
  <shadow distance="10" softness="5" opacity="50"/>
</layer>
```
If a shadow node exists, it will render a dropshadow on the parent layer using the attributes provided. If either of the attributes are left out, the shadow won't be drawn.

#### 'distance' attribute (Not optional)
Distance in pixels from the layer. The distance is always down to the right.

#### 'softness' attribute (Not optional)
Defines how soft the shadow will appear. A value of 0 is sharpest.

#### 'opacity' attribute (Not optional)
Defines the opacity of the shadow. 100 is completely visible. 0 is completely transparent.
