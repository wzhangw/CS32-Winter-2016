class File
{
public:
	File(string n) : m_name(n){}
	virtual ~File() {}
	string name() const { return m_name; }
	virtual void open() const = 0;
	virtual void redisplay() const { cout << "refresh the screen"; }
private:
	string m_name;
};

class TextMsg : public File
{
public:
	TextMsg(string n) : File(n) {}
	~TextMsg() { cout << "Destroying " << name() << ", a text message" << endl; }
	virtual void open() const { cout << "open text message"; }
};

class Video : public File
{
public:
	Video(string n, int time) : File(n), m_time(time) {}
	~Video() { cout << "Destroying " << name() << ", a video" << endl; }
	virtual void open() const { cout << "play " << m_time << " second video"; }
	virtual void redisplay() const { cout << "replay video"; }
private:
	int m_time;
};

class Picture : public File
{
public:
	Picture(string n) : File(n) {}
	~Picture() { cout << "Destroying the picture " << name() << endl; }
	virtual void open() const { cout << "show picture"; }
};