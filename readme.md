# Attribute Parser
This was a challenge completed on HackerRank.com. The challenge was the following:

>This challenge works with a custom-designed markup language HRML. In HRML, each element consists of a starting and ending tag, and there are attributes associated with each tag. Only starting tags can have attributes. We can call an attribute by referencing the tag, followed by a tilde, '~' and the name of the attribute. The tags may also be nested.

>The opening tags follow the format:

><tag-name attribute1-name = "value1" attribute2-name = "value2" ...>

>The closing tags follow the format:

></tag-name>

>The attributes are referenced as:

>tag1~value  
tag1.tag2~name

>Given the source code in HRML format consisting of  lines, answer  queries. For each query, print the value of the attribute specified. Print "Not Found!" if the attribute does not exist.

The following test input can be run with this code at it's Repl.it webpage (https://replit.com/@AndrewMiles/Attribute-Parser-Hackerrank-Challenge#main.cpp)
```
16 14
<tag1 v1 = "123" v2 = "43.4" v3 = "hello">
</tag1>
<tag2 v4 = "v2" name = "Tag2">
<tag3 v1 = "Hello" v2 = "World!">
</tag3>
<tag4 v1 = "Hello" v2 = "Universe!">
</tag4>
</tag2>
<tag5>
<tag7 new_val = "New">
</tag7>
</tag5>
<tag6>
<tag8 intval = "34" floatval = "9.845">
</tag8>
</tag6>
tag1~v1
tag1~v2
tag1~v3
tag4~v2
tag2.tag4~v1
tag2.tag4~v2
tag2.tag3~v2
tag5.tag7~new_val
tag5~new_val
tag7~new_val
tag6.tag8~intval
tag6.tag8~floatval
tag6.tag8~val
tag8~intval
```
