# define	Nb	4
# define	Nk	4
# define	Nr	10

typedef	union {
	unsigned long int	word;
	unsigned char	byte[4];
}	key_t;

void Cipher(unsigned char in[4*Nb], unsigned char out[4*Nb], unsigned long int w[Nb*(Nr+1)]);

void SubBytes(unsigned char state[4*Nb]);
void ShiftRows(unsigned char state[4*Nb]);
void MixColumns(unsigned char state[4*Nb]);
void AddRoundKey(unsigned char state[4*Nb], unsigned long int w[Nb]);
void KeyExpansion(unsigned char key[Nk], unsigned long int w[Nb*(Nr+1)]);

unsigned char Inverse(unsigned char b);
unsigned char Multiply(unsigned char x, unsigned char y);
unsigned char Affine(unsigned char b);

void PrintBlock(unsigned char state[4*Nb]);
void PrintKey(unsigned long int w[Nb]);
