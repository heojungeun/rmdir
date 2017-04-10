static const char *s = "abc";
int main() {
    
    return !(s[0] == 'a' && s[1] == 'b' && s[2] == 'c' && s[3] == '\0' && (s += 4));
}
