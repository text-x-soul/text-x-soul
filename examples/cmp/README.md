# Comparison:
## Between Soul and XML

The conclusion is that both Soul and XML are not comparable and function in a different way, but similar cases like value declarations can be compared.

Soul still has a long way to become a widely-acceptable format.

In most of these cases Soul took less space.

## Comparing by the number of lines
`Note that this is not much of a good comparison, as many people **disagree** in comparing lines of code. Rather comparing with how they work is what matters.`

But this could matter in larger files.

- Ignoring comments and spaces

`examples/theming/theme1.xml`
```xml
<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<design>
    <editorbackcolor>#31363b</editorbackcolor>
    <editortextcolor>#eff0f1</editortextcolor>
    <editorbordercolor>#b9b9b9</editorbordercolor>
    <linecolumnbackcolor>#484874</linecolumnbackcolor>
    <linecolumntextcolor>#dfdfdf</linecolumntextcolor>
    <linecolumnseparatorcolor>#b9b9b9</linecolumnseparatorcolor>
    <activelinecolor>#3daee9</activelinecolor>
    <intelliboxbackcolor>#fafafa</intelliboxbackcolor>
    <intelliboxtextcolor>#333333</intelliboxtextcolor>
    <intelliboxbordercolor>#b9b9b9</intelliboxbordercolor>
    <intelliboxselectionbackcolor>#c0dcf3</intelliboxselectionbackcolor>
    <intelliboxselectionbordercolor>#90c8f6</intelliboxselectionbordercolor>
    <intelliboxpressbackcolor>#90c8f6</intelliboxpressbackcolor>
    <intelliboxpressbordercolor>#60b0f9</intelliboxpressbordercolor>
    <editorborder>1 1 1 1</editorborder>
    <intelliboxborder>1 1 1 1</intelliboxborder>
    <linecolumnpadding>16 16</linecolumnpadding>
    <popupsize>400 300</popupsize>
    <haslinecolumn>true</haslinecolumn>
    <showfocusrect>false</showfocusrect>
    <firstlineone>true</firstlineone>
</design>
```
It takes up **24 lines of code** if you could count which has 1187 characters, making up 1187 bytes of data.

`examples/theming/theme1.soul`
```
¡ SOUL_VERSION = 0.5.2 ¡
editorbackcolor = #31363b
editortextcolor = #eff0f1
editorbordercolor = #b9b9b9
linecolumnbackcolor = #484874
linecolumntextcolor = #dfdfdf
linecolumnseparatorcolor = #b9b9b9
activelinecolor = #3daee9
intelliboxbackcolor = #fafafa
intelliboxtextcolor = #333333
intelliboxbordercolor = #b9b9b9
intelliboxselectionbackcolor = #c0dcf3
intelliboxselectionbordercolor = #90c8f6
intelliboxpressbackcolor = #90c8f6
intelliboxpressbordercolor = #60b0f9
editorborder = 1 1 1 1
intelliboxborder = 1 1 1 1
linecolumnpadding = 16 16
popupsize = 400 300
haslinecolumn = true
showfocusrect = false
firstlineone = true
```

It takes up **22 lines of code** if you could count which has 628 characters, making up 630 bytes of data.

## Comparing by serialization
- Ignoring spaces and comments

`examples/cmp/order.xml`
```xml
<PurchaseOrder xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"  xmlns:xsd="http://www.w3.org/2001/XMLSchema">
    <ItemsOrders>
        <Item>
            <ItemID>aaa111</ItemID>
            <ItemPrice>34.22</ItemPrice>
        </Item>
        <Item>
            <ItemID>bbb222</ItemID>
            <ItemPrice>2.89</ItemPrice>
        </Item>
    </ItemsOrders>
</PurchaseOrder>
```
Code is **contained** within **user-defined tags**, these cannot be **overriden within that group**. This code took about **12 lines of code** which has 384 characters, making up 384 bytes of data.

`example/cmp/order.soul`
```
PurchaseOrder() = {ItemsOrders}
ItemsOrders() = {Item}
Item1() = {ItemID, ItemPrice}
Item2() = {ItemID, ItemPrice}
ItemID@(Item1) = aaa111
ItemPrice@(Item1) = 34.22
ItemID@(Item2) = bbb222
ItemPrice@(Item2) = 2.89
```

Code is **not contained**. Values can be overriden anytime, anywhere in the file. This code took about **8 lines of code** which has 213 characters, making up 213 bytes of data.
