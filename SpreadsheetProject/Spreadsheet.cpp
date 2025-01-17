#include "Spreadsheet.hpp"
#include <utility>
#include <stdexcept>
#include <format>
#include <iostream>
#include <algorithm>
#include <chrono>

using namespace std;

class Spreadsheet::Impl
{
public:
	Impl(const SpreadsheetApplication& theApp,
		size_t width, size_t height);
	Impl(const Impl& src);
	Impl(Impl&&) noexcept = default;
	~Impl();
	Impl& operator=(const Impl& rhs);
	Impl& operator=(Impl&&) noexcept = default;
	
	void setCellAt(size_t x, size_t y, const SpreadsheetCell& cell);
	SpreadsheetCell& getCellAt(size_t x, size_t y);
	
	size_t getId() const;
	
	void swap(Impl& other) noexcept;

private:
	void verifyCoordinate(size_t x, size_t y) const;
	
	const size_t m_id{ 0 };
	size_t m_width{ 0 };
	size_t m_height{ 0 };
	SpreadsheetCell** m_cells{ nullptr };
	
	const SpreadsheetApplication& m_theApp;

	static inline size_t ms_counter{ 0 };
};

Spreadsheet::Impl::Impl(const SpreadsheetApplication& theApp,
	size_t width, size_t height)
	: m_id{ ms_counter++ }
	, m_width{min(width, Spreadsheet::MaxWidth)}
	, m_height{ min(height, Spreadsheet::MaxHeight) }
	, m_theApp {theApp}
{
	m_cells = new SpreadsheetCell * [m_width];
	for (size_t i{ 0 }; i < m_width; i++) {
		m_cells[i] = new SpreadsheetCell[m_height];
	}
}

Spreadsheet::Impl::~Impl()
{
	for (size_t i{ 0 }; i < m_width; i++) {
		delete[] m_cells[i];
	}
	delete[] m_cells;
	m_cells = nullptr;
}

Spreadsheet::Impl::Impl(const Impl& src)
	: Impl{ src.m_theApp, src.m_width, src.m_height }
{
	for (size_t i{ 0 }; i < m_width; i++) {
		for (size_t j{ 0 }; j < m_height; j++) {
			m_cells[i][j] = src.m_cells[i][j];
		}
	}
}

void Spreadsheet::Impl::verifyCoordinate(size_t x, size_t y) const
{
	if (x >= m_width) {
		throw out_of_range{ format("{} must be less than {}.", x, m_width) };
	}
	if (y >= m_height) {
		throw out_of_range{ format("{} must be less than {}.", y, m_height) };
	}
}

void Spreadsheet::Impl::setCellAt(size_t x, size_t y, const SpreadsheetCell& cell)
{
	verifyCoordinate(x, y);
	m_cells[x][y] = cell;
}

SpreadsheetCell& Spreadsheet::Impl::getCellAt(size_t x, size_t y)
{
	verifyCoordinate(x, y);
	return m_cells[x][y];
}

void Spreadsheet::Impl::swap(Impl& other) noexcept
{
	std::swap(m_width, other.m_width);
	std::swap(m_height, other.m_height);
	std::swap(m_cells, other.m_cells);
}

Spreadsheet::Impl& Spreadsheet::Impl::operator=(const Impl& rhs)
{
	Impl temp{ rhs };
	swap(temp);
	return *this;
}

size_t Spreadsheet::Impl::getId() const
{
	return m_id;
}

Spreadsheet::Spreadsheet(const SpreadsheetApplication& theApp,
	size_t width, size_t height)
{
	m_impl = make_unique<Impl>(theApp, width, height);
}

Spreadsheet::Spreadsheet(const Spreadsheet& src)
{
	m_impl = make_unique<Impl>(*src.m_impl);
}

Spreadsheet::~Spreadsheet() = default;
Spreadsheet::Spreadsheet(Spreadsheet&&) noexcept = default;
Spreadsheet& Spreadsheet::operator=(Spreadsheet&&) noexcept = default;

void Spreadsheet::setCellAt(size_t x, size_t y, const SpreadsheetCell& cell)
{
	m_impl->setCellAt(x, y, cell);
}

SpreadsheetCell& Spreadsheet::getCellAt(size_t x, size_t y)
{
	return m_impl->getCellAt(x, y);
}

size_t Spreadsheet::getId() const
{
	return m_impl->getId();
}

Spreadsheet& Spreadsheet::operator=(const Spreadsheet& rhs)
{
	*m_impl = *rhs.m_impl;
	return *this;
}

void Spreadsheet::swap(Spreadsheet& other) noexcept
{
	std::swap(m_impl, other.m_impl);
}

void swap(Spreadsheet& first, Spreadsheet& second) noexcept
{
	first.swap(second);
}
