// (C) LHMP Team 2013-2016; Licensed under Apache 2; See LICENSE;;

class CBitArray
{
public:
	// Create an empty instance
	CBitArray()
	{
		this->p_data = 0x0;
	}

	// Create instance using @data
	CBitArray(unsigned char data)
	{
		this->p_data = data;
	}

	// Toggle bit at @position to @state
	// position is <0,7> (byte has 8 bits, starting at 0)
	void SetBit(unsigned char position, bool state)
	{
		if (state == 1)
		{
			p_data |= 1 << position;
		}
		else {
			p_data &= ~(1 << position);
		}
	}
	
	// Return state of bit at @position
	// position is <0,7> (byte has 8 bits, starting at 0)
	bool GetBit(unsigned char position)
	{
		return (this->p_data >> position) & 1;
	}

	// Exports p_data
	unsigned char ExportData()
	{
		return this->p_data;
	}

	// Import data (same as constructor)
	void ImportData(unsigned char data)
	{
		this->p_data = data;
	}
private:
	// holds all 8 bools
	unsigned char p_data;
};