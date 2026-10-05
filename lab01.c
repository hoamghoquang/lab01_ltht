#include <stdio.h>

void PrintBits(unsigned int x) {
    int i;
    for (i = 8 * sizeof(x)-1; i >= 0; i--) {
        (x & (1 << i)) ? putchar('1') : putchar('0');
    }
    printf("\n");
}
void PrintBitsOfByte(unsigned int x) {
    int i;
    for (i = 7; i >= 0; i--) {
        (x & (1 << i)) ? putchar('1') : putchar('0');
    }
    printf("\n");
}

//1.1
//phep bu 2
int negative(int x){
    return ~x+1;
}

//1.2 Tính giá trị 50*x mà không được dùng phép nhân.
//day la 2^5*x + 2^4*x + 2^1
//nhan voi luy thua cua 2 thi dung phep dich trai
int cal50x(int x){
    x = (x<<5) + (x<<4) + (x<<1);
    return x;
}

//1.3 Trả về byte thứ n của x (các byte được đánh thứ tự từ 0 đến 3 từ phải sang trái).
//dich n lan ve ben phai va and voi 0xff
int getByte(int x,int n){
    x = x>>(n<<3);
    x &= 0b11111111;
    return x;
}
//1.4Lật 1 byte thứ n của số nguyên x (các byte được
// đánh theo thứ tự từ 0 đến 3 từ phải sang trái), từ
// bit 0 thành 1 và ngược lại.

int flipByte(int x,int n){
    x =x^ (0b11111111<<(n<<3));//dich byte
    return x;
}

//Tính kết quả x/2n
//day la *2^n
int divpw2(int x, int n){
    return x<<(~n+1);
}

//
int divpw2s(int x, int n)
{
    int sign = x >> 31;//check dấu
    int bias = (1 << n) + ~0;
    return ((x + (sign & bias)) >> n);// nếu dấu = âm thì mới cộng bias
}

// 2_1 
int isOpposite(int x, int y) {
    return (((x ^ y) >> 31) & 1)&!((~x + 1)^y);//kiểm tra x khác dấu y(để loại x=y=0), 
    //kiểm tra y đảo có bằng x (ko có vế trên thì về dưới sẽ trả 1 nếu x=y=0)

}

// 2_2 
int is16x(int x){
    return !(!(x & 16));//biến thành boolean
}

// 2_3
int isPositive(int x){
    return (!(!x))&(!((x >> 31)&1));
}

// 2_4
int isGE2n(int x,int n){
    x = x + (~(1 << n) + 1);
    int sign = (x >> 31) & 1;
    return sign ^1; //giong 1 thi la duong, (tuc la lown )
}

// 2_5
int subOK(int x, int y){
    //nếu x,y cùng dấu thì ko tràn (do phép trừ)
    int checksamesign = !((x>>31) ^ (y>>31));//check dấu
    y = ~y +1;
    return checksamesign | !(((x+y)>>31)^(x>>31));//so dấu với x sau khi trừ
}


//is parity
int bitParity(int x){
    x ^= x >> 16;
    x ^= x >> 8;
    x ^= x >> 4;
    x ^= x >> 2;
    x ^= x >> 1;
return x & 1;
}

// 3_1: 7 ops
unsigned float_negate(unsigned uf) {
    unsigned exp = (uf >> 23) & 0xFF;
    unsigned frac = uf &0x7FFFFF;

    // Nếu là NaN thì giữ nguyên
    // là NaN khi 
    //Exponent = 0b11111111
    //Fraction != 0b00000000000000000000000
    if (exp == 0b11111111 & frac != 0)
        return uf;

    // Đổi dấu
    return uf ^ 0x80000000;
}

// 3_2: 7 ops
unsigned float_absval(unsigned uf){
    unsigned exp = (uf >> 23) & 0xFF;
    unsigned frac = uf &0x7FFFFF;

    // Nếu là NaN thì giữ nguyên
    // là NaN khi 
    //Exponent = 0b11111111
    //Fraction != 0b00000000000000000000000
    if (exp == 0b11111111 & frac != 0)
        return uf;
    
    // cho bit dấu = 1
    return uf | 0x80000000 ; 
}

// 3_3
unsigned float_twice(unsigned uf)
{
    unsigned exp = (uf >> 23) & 0xFF;

    if (exp == 0xFF)// trường hợp vô cùng và nan
        return uf;

    if (exp == 0)// trường hợp zero hoặc subnormal
        return (uf & 0x80000000)/*lưu bit dấu phòng trường hợp exp tràn qua bit*/ 
        | (uf << 1);

    if (exp == 0xFE)  // Nhân 2 sẽ overflow  và trả về Infinity
        return (uf & 0x80000000)/*kiểm tra dấu*/ | 0x7F800000;// trả về infinity

    //trường hợp normal
    // tăng số mũ lên 1 (2^n * 2)
    return uf + 0x00800000;
}

//3_4
int float_f2i(unsigned uf) {
    unsigned s = uf >> 31;
    unsigned exp = (uf >> 23) & 0xFF;
    unsigned frac = uf & 0x7FFFFF;
    int E;
    int result;

    // 1. Trường hợp exp = 0  giá trị < 1, làm tròn về 0
    if (exp == 0) {
        return 0;
    }

    // 2. Trường hợp exp = 0xFF (Vô cực hoặc NaN): trả về 0x80000000 theo yêu cầu
    if (exp == 0xFF) {
        return 0x80000000;
    }

    // Tính số mũ thực tế
    E = (int)exp - 127;

    // 3. Nếu E < 0, trị tuyệt đối của số < 1, làm tròn về 0
    if (E < 0) {
        return 0;
    }

    // 4. Nếu E >= 31, vượt quá giới hạn chứa của số nguyên int 32-bit
    if (E >= 31) {
        return 0x80000000;
    }

    // Khôi phục phần định trị đầy đủ có chứa bit 1 ngầm định ở vị trí 23: 1.frac
    unsigned int significand = (1 << 23) | frac;

    // Dịch bit tương ứng với số mũ E để đưa về dạng số nguyên
    if (E > 23) {
        result = significand << (E - 23);
    } else {
        result = significand >> (23 - E);
    }

    // 5. Nếu là số âm (s == 1), đảo dấu kết quả
    if (s) {
        result = -result;
    }

    return result;
}

int main(){

}
