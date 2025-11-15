#include <gtest/gtest.h>
#include "Strings/String.h"
#include "LeakDetector/LeakDetector.h"

#include <iostream>

using namespace Strings;

TEST(StringTests, Constructors) {
    StringFixedSize<8> string {"HELLO"};
    EXPECT_EQ(string.length(), 5 );
    EXPECT_STREQ(string.c_str(), "HELLO" );

    // Copy constructor
    {
        StringFixedSize<8> str2 {string};
        EXPECT_EQ(str2.length(), 5 );
        EXPECT_STREQ(str2.c_str(), "HELLO" );
    }

    // Default constructor and assignment
    {
        StringFixedSize<8> str2 {};
        EXPECT_EQ(str2.length(), 0 );
        EXPECT_STREQ(str2.c_str(), "" );

        str2 = string;
        EXPECT_EQ(str2.length(), 5 );
        EXPECT_STREQ(str2.c_str(), "HELLO" );

        // Move assignment
        str2 = static_cast<StringFixedSize<8>&&>(string);
        EXPECT_EQ(str2.length(), 5 );
        EXPECT_STREQ(str2.c_str(), "HELLO" );
        EXPECT_EQ(string.length(), 0 );
        EXPECT_STREQ(string.c_str(), "" );

        // Move back into original object
        string = static_cast<StringFixedSize<8>&&>(str2);
        EXPECT_EQ(string.length(), 5 );
        EXPECT_STREQ(string.c_str(), "HELLO" );
        EXPECT_EQ(str2.length(), 0 );
        EXPECT_STREQ(str2.c_str(), "" );
    }

    // Move constructor
    {
        StringFixedSize<8> str2 {static_cast<StringFixedSize<8>&&>(string)};
        EXPECT_EQ(str2.length(), 5 );
        EXPECT_STREQ(str2.c_str(), "HELLO" );
        EXPECT_EQ(string.length(), 0 );
        EXPECT_STREQ(string.c_str(), "" );
    }
}

TEST(StringTests, SaturateAfterInsert) {
    StringFixedSize<8> string {"HELLO"};

    // Insert beyond available capacity. String must saturate.
    string.insert(string.size(), " WORLD!");

    EXPECT_EQ(string.length(), 8 );
    EXPECT_STREQ(string.c_str(), "HELLO WO" );
}

TEST(StringTests, SaturateAfterInsertMiddle) {
    StringFixedSize<8> string {"12345678"};

    // Insert inside the string, shifting existing characters
    string.insert(4, 2, '0' );
    EXPECT_STREQ(string.c_str(), "12340056" );

    // Insert more characters than available space
    string.insert(4, 8, ' ' );
    EXPECT_STREQ(string.c_str(), "1234    " );
}

TEST(StringTests, FindBasicChars) {
    // String containing an embedded null character
    StringFixedSize<8> string {"0123\0ABC", 8};

    EXPECT_EQ(string.find('0'), 0 );
    EXPECT_EQ(string.find('1'), 1 );
    EXPECT_EQ(string.find('2'), 2 );
    EXPECT_EQ(string.find('3'), 3 );
    EXPECT_EQ(string.find('\0'), 4 );
    EXPECT_EQ(string.find('A'), 5 );
    EXPECT_EQ(string.find('B'), 6 );
    EXPECT_EQ(string.find('C'), 7 );
}

TEST(StringTests, FindBasicSubstrings) {
    StringFixedSize<8> string {"0123\0ABC", 8};

    // Substring containing null characters
    EXPECT_EQ(string.find("3\0AB"), 3 );
    EXPECT_EQ(string.find("AB"), 5 );

    // Not found conditions
    EXPECT_EQ(string.find("99"), npos );
    EXPECT_EQ(string.find("ABCDEFGHIJ"), npos );
}

TEST(StringTests, FindWithExplicitCount) {
    StringFixedSize<8> string {"0123\0ABC", 8};

    // Count limits the number of characters considered from 's'
    EXPECT_EQ(string.find("0123", 0, 4), 0 );
    EXPECT_EQ(string.find("0123", 1, 4), npos );
}

TEST(StringTests, FindEdgeCases) {
    StringFixedSize<8> string {"0123\0ABC", 8};

    // Count larger than string length
    EXPECT_EQ(string.find("0123456789", 0, 10), npos );

    StringFixedSize<8> s2 {"AAAABBBB", 8};
    EXPECT_EQ(s2.find('A'), 0 );
    EXPECT_EQ(s2.find('B'), 4 );
}

TEST(StringTests, RFindWithOtherString) {
    StringFixedSize<8> string {"0123\0ABC", 8};
    StringFixedSize<8> key {"3\0A", 3};

    // Search using IBasicString overload
    EXPECT_EQ(string.find(key), 3 );
    EXPECT_EQ(string.find(key,4), npos );
}

TEST(StringTests, RFindBasicChars) {
    StringFixedSize<8> string {"0123\0ABC", 8};

    EXPECT_EQ(string.rfind('0'), 0 );
    EXPECT_EQ(string.rfind('1'), 1 );
    EXPECT_EQ(string.rfind('2'), 2 );
    EXPECT_EQ(string.rfind('3'), 3 );
    EXPECT_EQ(string.rfind('\0'), 4 );
    EXPECT_EQ(string.rfind('A'), 5 );
    EXPECT_EQ(string.rfind('B'), 6 );
    EXPECT_EQ(string.rfind('C'), 7 );

    EXPECT_EQ(string.rfind('Z'), npos );
}

TEST(StringTests, RFindSubstrings) {
    StringFixedSize<8> string {"0123\0ABC", 8};

    EXPECT_EQ(string.rfind("3\0AB"), 3 );      
    EXPECT_EQ(string.rfind("AB"), 5 );
    EXPECT_EQ(string.rfind("99"), npos );
    EXPECT_EQ(string.rfind("ABCDEFGHIJ"), npos );
}

TEST(StringTests, RFindWithExplicitCount) {
    StringFixedSize<8> string {"0123\0ABC", 8};

    // Search backwards with a limited substring length
    EXPECT_EQ(string.rfind("0123", npos, 4), 0 );
    EXPECT_EQ(string.rfind("0123", 3, 4), 0 ); 
    EXPECT_EQ(string.rfind("0123", 2, 4), 0 ); 
}

TEST(StringTests, RFindWithPositionLimit) {
    StringFixedSize<8> string {"0123\0ABC", 8};

    EXPECT_EQ(string.rfind("AB", 7), 5 );
    EXPECT_EQ(string.rfind("AB", 5), 5 );
    EXPECT_EQ(string.rfind("AB", 4), npos );
}

TEST(StringTests, RFindWithIBasicString) {
    StringFixedSize<8> string {"0123\0ABC", 8};
    StringFixedSize<8> key {"3\0A", 3};

    EXPECT_EQ(string.rfind(key), 3 );
    EXPECT_EQ(string.rfind(key, 2), npos );
}

TEST(StringTests, RFindCharacterWithPositionLimit) {
    StringFixedSize<8> string {"0123\0ABC", 8};

    EXPECT_EQ(string.rfind('A', 7), 5 );
    EXPECT_EQ(string.rfind('A', 5), 5 );
    EXPECT_EQ(string.rfind('A', 4), npos );
}

TEST(StringTests, RFindEdgeCases) {
    StringFixedSize<8> string {"0123\0ABC", 8};

    EXPECT_EQ(string.rfind("0123456789", 0, 10), npos );

    // Position beyond the string should still search normally
    EXPECT_EQ(string.rfind('C', 999), 7 );

    StringFixedSize<8> s2 {"AAAABBBB", 8};
    EXPECT_EQ(s2.rfind('A'), 3 );
    EXPECT_EQ(s2.rfind('B'), 7 );
}

TEST(StringTests, FindFirstOf_SingleChar) {
    StringFixedSize<8> str{"0123\0ABC", 8};

    EXPECT_EQ(str.find_first_of('0'), 0);
    EXPECT_EQ(str.find_first_of('3'), 3);
    EXPECT_EQ(str.find_first_of('\0'), 4);
    EXPECT_EQ(str.find_first_of('A'), 5);
    EXPECT_EQ(str.find_first_of('C'), 7);

    // Character not found
    EXPECT_EQ(str.find_first_of('Z'), npos);

    // Position offset
    EXPECT_EQ(str.find_first_of('1', 2), npos);
    EXPECT_EQ(str.find_first_of('A', 6), npos);
    EXPECT_EQ(str.find_first_of('A', 5), 5);
}

TEST(StringTests, FindFirstOf_StringSet) {
    StringFixedSize<8> str{"0123\0ABC", 8};

    EXPECT_EQ(str.find_first_of("ABC"), 5);
    EXPECT_EQ(str.find_first_of("3A"), 3);
    EXPECT_EQ(str.find_first_of("C"), 7);

    // Includes null character
    EXPECT_EQ(str.find_first_of("X\0Y", 0, 3), 4);

    // No match
    EXPECT_EQ(str.find_first_of("XYZ"), npos);
}

TEST(StringTests, FindFirstOf_SubsetWithCount) {
    StringFixedSize<8> str{"0123\0ABC", 8};

    EXPECT_EQ(str.find_first_of("012345", 0, 2), 0);
    EXPECT_EQ(str.find_first_of("012345", 2, 3), 2);

    // Count smaller than buffer
    EXPECT_EQ(str.find_first_of("3\0A---", 0, 3), 3);

    // Starting position
    EXPECT_EQ(str.find_first_of("ABC", 6), 6);
    EXPECT_EQ(str.find_first_of("ABC", 7), 7);
    EXPECT_EQ(str.find_first_of("ABC", 8), npos);
}

TEST(StringTests, FindFirstOf_EmptyOrInvalidInput) {
    StringFixedSize<8> str{"01234567", 8};

    // No characters in the set
    EXPECT_EQ(str.find_first_of("ABC", 0, 0), npos);

    // Position out of range
    EXPECT_EQ(str.find_first_of("ABC", 10), npos);
}

TEST(StringTests, FindFirstNotOf_SingleChar) {
    StringFixedSize<8> str{"0123\0ABC", 8};

    EXPECT_EQ(str.find_first_not_of('0'), 1);
    EXPECT_EQ(str.find_first_not_of('1'), 0);
    EXPECT_EQ(str.find_first_not_of('\0'), 0);

    // Position offset
    EXPECT_EQ(str.find_first_not_of('3', 3), 4);
    EXPECT_EQ(str.find_first_not_of('A', 5), 6);

    // No non-matching character left
    EXPECT_EQ(str.find_first_not_of('A', 6), 6);
}

TEST(StringTests, FindFirstNotOf_StringSet) {
    StringFixedSize<8> str{"0123\0ABC", 8};

    EXPECT_EQ(str.find_first_not_of("0123"), 4);
    EXPECT_EQ(str.find_first_not_of("0123\0", 0, 5), 5);
    EXPECT_EQ(str.find_first_not_of("ABC"), 0);

    // Set includes the null character
    EXPECT_EQ(str.find_first_not_of("X\0Y", 0, 3), 0);

    // All except last character belong to the set
    EXPECT_EQ(str.find_first_not_of("0123\0AB", 0, 7), 7);
}

TEST(StringTests, FindFirstNotOf_SubsetWithCount) {
    StringFixedSize<8> str{"0123\0ABC", 8};

    EXPECT_EQ(str.find_first_not_of("012345", 0, 2), 2);
    EXPECT_EQ(str.find_first_not_of("3\0A---", 3, 3), 6);

    // Entire remaining region matches the set
    EXPECT_EQ(str.find_first_not_of("ABC", 5), npos);
}

TEST(StringTests, FindFirstNotOf_EmptyOrInvalidInput) {
    StringFixedSize<8> str{"01234567", 8};

    // Empty set means the first character is not in the set
    EXPECT_EQ(str.find_first_not_of("ABC", 0, 0), 0);

    // Position out of range
    EXPECT_EQ(str.find_first_not_of("ABC", 10), npos);
}

TEST(StringTests, FindLastOf_SingleChar) {
    // String includes an embedded null character
    StringFixedSize<8> str{"0123\0ABC", 8};

    // Basic checks for individual characters
    EXPECT_EQ(str.find_last_of('0'), 0);
    EXPECT_EQ(str.find_last_of('3'), 3);
    EXPECT_EQ(str.find_last_of('\0'), 4);
    EXPECT_EQ(str.find_last_of('A'), 5);
    EXPECT_EQ(str.find_last_of('C'), 7);

    // Character not found
    EXPECT_EQ(str.find_last_of('Z'), npos);

    // Limit search position
    EXPECT_EQ(str.find_last_of('A', 4), npos);
    EXPECT_EQ(str.find_last_of('A', 5), 5);
    EXPECT_EQ(str.find_last_of('C', 6), npos);
}

TEST(StringTests, FindLastOf_StringSet) {
    StringFixedSize<8> str{"0123\0ABC", 8};

    // Set "ABC" -> last match is 'C' at position 7
    EXPECT_EQ(str.find_last_of("ABC"), 7);

    // Set "3A" -> last match is 'A' at position 5
    EXPECT_EQ(str.find_last_of("3A"), 5);

    // Set containing the null character
    EXPECT_EQ(str.find_last_of("X\0Y", npos, 3), 4);

    // None of the characters appear
    EXPECT_EQ(str.find_last_of("XYZ"), npos);
}

TEST(StringTests, FindLastOf_WithExplicitCount) {
    StringFixedSize<8> str{"0123\0ABC", 8};

    // Restrict the set to only '0' and '1'
    EXPECT_EQ(str.find_last_of("012345", npos, 2), 1);

    // Search from middle of the string
    EXPECT_EQ(str.find_last_of("3\0A---", 6, 3), 5);

    // Position-limited search
    EXPECT_EQ(str.find_last_of("ABC", 6), 6);
    EXPECT_EQ(str.find_last_of("ABC", 5), 5);
    EXPECT_EQ(str.find_last_of("ABC", 4), npos);
}

TEST(StringTests, FindLastOf_EmptyOrInvalidInput) {
    StringFixedSize<8> str{"01234567", 8};

    // Empty set (count = 0): nothing can match
    EXPECT_EQ(str.find_last_of("ABC", 0, 0), npos);

    // Position outside valid range
    EXPECT_EQ(str.find_last_of("ABC", 100), npos);
}

TEST(StringTests, FindLastNotOf_SingleChar) {
    // String with embedded null
    StringFixedSize<8> str{"0123\0ABC", 8};

    // Looking for last char NOT equal to given character
    EXPECT_EQ(str.find_last_not_of('0'), 7);   // last not '0' is 'C' at pos 7
    EXPECT_EQ(str.find_last_not_of('3'), 7);   // last not '3' is 'C'
    EXPECT_EQ(str.find_last_not_of('\0'), 7);  // last not null is 'C'
    EXPECT_EQ(str.find_last_not_of('A'), 7);   // last not 'A' is 'C'
    EXPECT_EQ(str.find_last_not_of('C'), 6);   // last not 'C' is 'B'

    // If all characters are the same as input, return npos
    StringFixedSize<8> allA{"AAAAAAAA", 8};
    EXPECT_EQ(allA.find_last_not_of('A'), npos);

    // Limit search position
    EXPECT_EQ(str.find_last_not_of('C', 6), 6); // search only up to pos 6
    EXPECT_EQ(str.find_last_not_of('C', 5), 5);
    EXPECT_EQ(str.find_last_not_of('C', 3), 3);
}

TEST(StringTests, FindLastNotOf_StringSet) {
    StringFixedSize<8> str{"0123\0ABC", 8};

    // Set "0123" -> last char not in set is 'C' at pos 7
    EXPECT_EQ(str.find_last_not_of("0123"), 7);

    // Set "ABC" -> last char not in set is null at pos 4
    EXPECT_EQ(str.find_last_not_of("ABC"), 4);

    // Set including null -> last char not in set is 'C' at pos 7
    EXPECT_EQ(str.find_last_not_of("X\0Y", npos, 3), 7);

    // Entire string in set -> return npos
    StringFixedSize<8> str2{"ABCABCAB", 8};
    EXPECT_EQ(str2.find_last_not_of("ABC"), npos);
}

TEST(StringTests, FindLastNotOf_WithExplicitCount) {
    StringFixedSize<8> str{"0123\0ABC", 8};

    // Restrict the set to only '0' and '1'
    EXPECT_EQ(str.find_last_not_of("012345", npos, 2), 7); // last not '0' or '1' is 'C'

    // Search from middle of string
    EXPECT_EQ(str.find_last_not_of("3\0A---", 6, 3), 6); // last not in set up to pos 6 is 'B'

    // Position-limited search
    EXPECT_EQ(str.find_last_not_of("ABC", 7), 4); // last not in set up to pos 7 is '\0'
    EXPECT_EQ(str.find_last_not_of("ABC", 4), 4); // last not in set up to pos 4 is '\0'
    EXPECT_EQ(str.find_last_not_of("ABC", 3), 3); // pos 3 itself
}

TEST(StringTests, FindLastNotOf_EmptyOrInvalidInput) {
    StringFixedSize<8> str{"01234567", 8};

    // Empty set (count = 0): all characters are considered not in set
    EXPECT_EQ(str.find_last_not_of("ABC", 0, 0), 0);   // only first char available

    // Position out of range -> use last character
    EXPECT_EQ(str.find_last_not_of("0123", 100), 7);   // last char '7' not in set
}
