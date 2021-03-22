extern "C" int Printf(const char* format, ...);

int main()
{
    Printf("I %s %x %s %s %d%%%c%b", "love", 3802, "and", "you", 100, 33, 255);
}