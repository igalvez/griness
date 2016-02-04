#define lowBank 0xC000;
#define highBank 0x8000;
#define BANK_SIZE 16*1024


Class MMC1:Mapper {
	private:
		uint8 shift_register;
		uint8 controlReg;
		int switchBank; // current bank to be switched, lowBank, highBank or 0 if both
	public:
		void clear();
		void write_control();
}
		
	
