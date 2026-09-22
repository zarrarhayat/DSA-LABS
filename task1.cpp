#include <iostream>
#include <cctype>
using namespace std;

string clean (string s){
    string result = "";
    for (int i =0;i<s.length();i++){
        if(isalnum(s[i])){
            result += tolower(s[i]);
        }
    }
    return result;
}
bool isPalindrome(string s){
    int left = 0;
    int right = s.length() -1;
    while(left<right ){
        if (s[left] != s[right]){
            return false;
            left++;
            right--;
            
        }
            return true;
         }
}
int main(){
    string input;
    cout<< "enter a string ";
    getline(cin,input);

    string processed= clean(input);

    if(isPalindrome(processed)){
        cout << input << " is a palindrome" << endl;
    }
    else {
        cout<< input <<" is not a palindrome "<< endl;
     return 0;
    
    }
}