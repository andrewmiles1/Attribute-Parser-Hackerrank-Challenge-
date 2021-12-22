/*
Read more about the challenge here:
https://www.hackerrank.com/challenges/attribute-parser/problem
*/

#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <map>

using namespace std;

//removes requested chars from string
void RemoveCharAll(string & str, char unwantedChar) {
  for (string::iterator iter = str.begin(); iter != str.end(); iter++) {
    if ( *iter == unwantedChar) {
      str.erase(iter);
      iter--; //go back one, since we removed.
    }
  }
}

class HRMLDocument {

  private:
    struct TagNode {
      string name;
      map < string, string > attributes;
      vector < TagNode > children;
      TagNode *pParent = nullptr;
    };
  TagNode *pHeadNode;
  TagNode *pTagCursor; //used as we add where we are when parsing
  vector < TagNode > fullHierarchy;

  public:
    HRMLDocument() {
      pHeadNode = nullptr;
      pTagCursor = nullptr;
      //we will have at least one
    }
  void AddTagAtCursor(string tagName) {
    if (pTagCursor == nullptr) {
      //if we're at base level
      //add node at base level
      fullHierarchy.push_back(TagNode());

      //set our cursor to new node
      pTagCursor = & *fullHierarchy.end() - 1;
      pTagCursor -> name = tagName;
      pTagCursor -> pParent = nullptr;
    } else {
      //add tag as child.
      pTagCursor -> children.push_back(TagNode());
      //hold a temp reference to created child
      TagNode *pTempTag = & *pTagCursor -> children.end() - 1;
      //set tag members
      pTempTag -> name = tagName;
      pTempTag -> pParent = pTagCursor;
      //move cursor to now current tag
      pTagCursor = pTempTag;
    }
  };
  void MoveCursorUp() {
    //if parent isn't non existant (that would mean we're at head node)
    if (pTagCursor != nullptr) {
      //move cursor up to parent.
      pTagCursor = pTagCursor -> pParent;
    }
  };
  void AddAttributeAtCursor(string att, string val) {
    pTagCursor -> attributes.emplace(att, val);
  };
  string GetAttributeValue(string query) {
    vector < string > simpQuery = SimplifyQuery(query); //turn query to vector
    string tempName;
    for (vector < TagNode > ::iterator it = fullHierarchy.begin(); it != fullHierarchy.end(); ++it) {
      //if we've found the head node of the query
      if (it -> name == *simpQuery.begin()) {
        if (simpQuery.size() > 2) { //deeper than surface level
          simpQuery.erase(simpQuery.begin()); //get rid of stepped item
          for (auto & val: it -> children) {
            if (val.name == *simpQuery.begin()) {
              return GetAttributeValueHelper(simpQuery, val);
            }
          }
          return "Not Found!";
          //find child with next tag
          //call helper passing query and reference to next tag
        } else {
          if (it -> attributes.find( *(simpQuery.end() - 1)) != it -> attributes.end()) {
            return it -> attributes.at( *(simpQuery.end() - 1)); //found it, return it.
          } else {
            return "Not Found!"; //att doesn't exist.
          }
        }

      }
    }
    return "Not Found!";
  };

  //Display the full HRML document parsed in
  void DisplayFullContents() { //RECURSIVE
    cout << "Full Tag Hierarchy:" << endl;

    //loop through vectors in full hierarchy and recursively display them:
    for (vector < TagNode > ::iterator it = fullHierarchy.begin(); it != fullHierarchy.end(); it++) {
      //call recursive function for stepping through tree
      DisplayFullContentsHelper( & * it, 0);
    }
  };
  private:
    void DisplayFullContentsHelper(TagNode *currentNode, int lvCount) { //RECURSIVE HELPER
      //indent
      for (int i = 0; i < lvCount; i++) {
        cout << "  ";
      }

      cout << "<" << currentNode -> name;
      for (map < string, string > ::iterator it = currentNode -> attributes.begin(); it != currentNode -> attributes.end(); ++it) {
        cout << " " << it -> first << " = \"" << it -> second << "\" ";
      }
      cout << ">" << endl;
      lvCount++;
      for (vector < TagNode > ::iterator it = currentNode -> children.begin(); it != currentNode -> children.end(); it++) {
        DisplayFullContentsHelper( & * it, lvCount);
      }
      //indent
      for (int i = 0; i < lvCount - 1; i++) {
        cout << "  ";
      }
      cout << "</" << currentNode -> name << ">" << endl;
    };
  string GetAttributeValueHelper(vector < string > remainingQuery, TagNode & node) {
    if (remainingQuery.size() > 2) {
      //we need to go a layer deeper
      remainingQuery.erase(remainingQuery.begin()); //remove current layer
      for (auto & val: node.children) {
        if (val.name == * remainingQuery.begin()) {
          return GetAttributeValueHelper(remainingQuery, val);
        }
      }
      return "Not Found!";
    } else {
      if (node.attributes.find( *(remainingQuery.end() - 1)) != node.attributes.end()) {
        //if requested attribute exists in this tag
        //return tag.
        return node.attributes.at( *(remainingQuery.end() - 1));
      } else {
        //not found!
        return "Not Found!";
      }
    }
  }
  vector < string > SimplifyQuery(string query) {
    int pos;
    query.replace(query.find('~'), 1, ".");
    vector < string > simpQuery;
    while ((pos = query.find('.')) != string::npos) {
      simpQuery.push_back(query.substr(0, pos)); //add to simp query
      query.erase(0, pos + 1); //erase item from front
    }
    //still tag~att left in query
    simpQuery.push_back(query); //all that's left
    return simpQuery;
  };

};

int main() {
  HRMLDocument parsedDoc;
  int hrmlLinesCount, queryCount;
  string tagName, attName, attValue;

  //get line count input
  cin >> hrmlLinesCount >> queryCount;

  //parse tags being input
  for (int i = 0; i < hrmlLinesCount; i++) {

    cin >> tagName;
    RemoveCharAll(tagName, '<');

    if (tagName.find('/') == 0) {
      parsedDoc.MoveCursorUp();
      continue;
    }

    if (tagName.find(">") != string::npos) {
      RemoveCharAll(tagName, '>');
      parsedDoc.AddTagAtCursor(tagName);
      continue; //no need to find attributes, all done
    } else {
      //not done yet
      parsedDoc.AddTagAtCursor(tagName);
    }

    do {
      cin >> attName >> tagName >> attValue;
      tagName = attValue; //I'm really using this as a temp value.
      RemoveCharAll(attValue, '\"');
      RemoveCharAll(attValue, '>');
      parsedDoc.AddAttributeAtCursor(attName, attValue);
    } while (tagName.find(">") == string::npos);
  }

  //parsedDoc.DisplayFullContents();

  //parse attribute queries and output value
  for(int i = 0; i < queryCount; i++){
    cin >> tagName;
    cout << parsedDoc.GetAttributeValue(tagName) << endl;
  }

  return 0;
}

/*
Test Input:

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

*/