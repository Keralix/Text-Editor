#include <string>
class Text{
public:
    Text(const std::string input);
    Text();
    ~Text();
    std::string getText() const;
    void changeText(const std::string& newText);
    void append(char c);
    void deleteLast();
    void clear();
    int length() const;
    void insertAt(int pos,char c);
    void deleteAt(int pos);
    void deleteBefore(int pos);
    void deleteAfter(int pos);
    char getCharAt(int pos);
private:
    std::string data;
};