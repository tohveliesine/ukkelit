#pragma once

#include <iostream>
#include <string>

class ConsoleOutput {
	std::ostream& _out;
	unsigned int _width;
	int _marginh;
	std::string _marginh_custom_left_str;

	unsigned int width() const { return _width; }
	unsigned int width_effective() const { return width() - (marginh() * 2); }

	void write_line(const std::string& text, bool use_custom);
	void write(const std::string& text, bool use_custom);

	public:
	ConsoleOutput(std::ostream& out, unsigned int width);
	ConsoleOutput(std::ostream& out, unsigned int width, int marginh);
	ConsoleOutput(std::ostream& out, unsigned int width, int marginh, const std::string& custom_margin);
	~ConsoleOutput();

	// write an empty new line
	void write_line();

	// write the given string to a new line,
	// with respect to the margins on new lines and wrapping.
	void write_line(const std::string& text);

	void write(const std::string& text);

	int marginh() const { return _marginh; }
	ConsoleOutput marginh(int marginh) const { return ConsoleOutput(_out, _width, _marginh + marginh); }

	std::string marginh_str() const { return marginh_str(true); }
	std::string marginh_str(bool use_custom) const;

	void write_center(const std::string& text);
	void write_fill(const std::string& left_text, const std::string& middle_text, const std::string& right_text);

	ConsoleOutput marginh_custom_left_str(const std::string& custom) {
		return marginh_custom_left_str(custom, custom.length());
	}
	ConsoleOutput marginh_custom_left_str(const std::string& custom, int marginh) {
		return ConsoleOutput(_out, _width, _marginh + marginh, custom);
	}
};
