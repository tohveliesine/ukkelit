#include <sstream>
#include <cassert>
#include <algorithm>

#include "ConsoleOutput.h"

static const std::string NEWLINE = "\n";

ConsoleOutput::ConsoleOutput(std::ostream& out, unsigned int width) : _out(out), _width(width), _marginh(0) {}
ConsoleOutput::ConsoleOutput(std::ostream& out, unsigned int width, int marginh) : ConsoleOutput(out, width) {
	_marginh = marginh;

	assert(marginh * 2 < (int)width);
}

ConsoleOutput::ConsoleOutput(std::ostream& out, unsigned int width, int marginh, const std::string& custom_margin)
    : ConsoleOutput(out, width, marginh) {
	_marginh_custom_left_str = custom_margin;
}

ConsoleOutput::~ConsoleOutput() {}

void ConsoleOutput::write_line() { write_line(""); }

// Returns the position in the string where to apply line break to.
// Breaks at word boundaries if possible.
static int get_ideal_linebreak(const std::string& line, int maxlen) {
	int wordbreak;
	int last_space = line.find_last_of(' ', maxlen);
	if (last_space == std::string::npos) {
		// not found! break at end
		wordbreak = maxlen;
	} else {
		// space found, must be after the half length, or we break at end-of-line
		if (last_space > maxlen / 2) {
			wordbreak = last_space;
		} else {
			wordbreak = maxlen;
		}
	}

	return wordbreak;
}

std::string ConsoleOutput::marginh_str(bool use_custom) const {
	if (!_marginh_custom_left_str.empty() && use_custom) {
		return std::string(marginh() - _marginh_custom_left_str.length(), ' ') + _marginh_custom_left_str;
	}

	return std::string(marginh(), ' ');
};

void ConsoleOutput::write(const std::string& text) { write(text, true); }
void ConsoleOutput::write(const std::string& text, bool use_custom) {
	// break text into lines
	std::istringstream is(text);

	// for each line, check that the width is not over effective width
	std::string line;
	while (std::getline(is, line)) {
		if (line.length() <= width_effective()) {
			// if not over, write: margin + line
			_out << marginh_str(use_custom) << line;
		} else {
			// if over, write: margin + line + NEWLINE + rest of the line (recursive)
			int wordbreak = get_ideal_linebreak(line, width_effective());
			_out << marginh_str(use_custom) << line.substr(0, wordbreak) << NEWLINE;
			write(line.substr(wordbreak + 1), false); // following lines do not use "custom" left margin
		}
	}
}

void ConsoleOutput::write_line(const std::string& text) { write_line(text, true); }
void ConsoleOutput::write_line(const std::string& text, bool use_custom) {
	// empty line = newline
	if (text.length() == 0) {
		_out << NEWLINE;
		return;
	}

	// break text into lines
	std::istringstream is(text);

	// for each line, check that the width is not over effective width
	bool first_and_use_custom = use_custom;
	std::string line;
	while (std::getline(is, line)) {
		if (line.length() <= width_effective()) {
			// if not over, write: margin + line + NEWLINE
			_out << marginh_str(first_and_use_custom) << line << NEWLINE;
		} else {
			// if over, write: margin + line + NEWLINE + rest of the line (recursive)
			int wordbreak = get_ideal_linebreak(line, width_effective());
			_out << marginh_str(first_and_use_custom) << line.substr(0, wordbreak) << NEWLINE;
			write_line(line.substr(wordbreak + 1),
			           false); // following lines do not use "custom" left margin
		}
		first_and_use_custom = false;
	}
}

void ConsoleOutput::write_center(const std::string& text) { write_fill("", text, ""); }
void ConsoleOutput::write_fill(const std::string& left_text, const std::string& middle_text,
                               const std::string& right_text) {
	// so we don't write too long stuff that doesn't fit on screen
	assert(left_text.length() + middle_text.length() + right_text.length() + (marginh() * 2) <= width());

	assert(left_text.length() + marginh() + (middle_text.length() / 2) <= width() / 2);
	assert(right_text.length() + marginh() + (middle_text.length() / 2) <= width() / 2);

	// calculate spaces after left text
	int left_justify = (width() / 2) - marginh() - left_text.length() - (middle_text.length() / 2);
	// calculate spaces after center to right-justify the text
	int right_justify = (width() / 2) - (middle_text.length() / 2 + middle_text.length() % 2) - right_text.length() - marginh();

	_out << marginh_str() << left_text << std::string(left_justify, ' ') << middle_text << std::string(right_justify, ' ')
	     << right_text << NEWLINE;
}