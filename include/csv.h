#pragma once

#include <concepts>
#include <fstream>
#include <functional>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>

template <class T, class U>
concept Derived = std::is_base_of<U, T>::value;

template <typename T> class CsvEntryBuilder {
public:
  T deserialize(const std::vector<std::string> &values) const;
  std::vector<std::string> serialize(const T &entry) const;
};

template <typename T, Derived<CsvEntryBuilder<T>> U> class CsvFile {
private:
  std::string path;
  std::vector<T> entries;

  U builder;
  bool modified = false;

public:
  CsvFile(std::string path) : path{path} {
    std::ifstream file(path);
    if (!file.is_open()) {
      throw std::runtime_error("Could not open file: " + path);
    }

    std::string line;
    while (std::getline(file, line)) {
      if (line.empty()) {
        continue;
      }

      std::vector<std::string> values;

      std::string_view line_view(line);
      for (const auto value : std::views::split(line_view, ',')) {
        values.push_back(std::string(value.begin(), value.end()));
      }

      T entry = builder.deserialize(values);

      entries.push_back(entry);
    }
  };

  ~CsvFile() {
    if (modified) {
      std::ofstream file(path);
      if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + path);
      }

      for (const auto &entry : entries) {
        auto values = builder.serialize(entry);
        for (size_t i = 0; i < values.size(); ++i) {
          file << values[i];
          if (i != values.size() - 1) {
            file << ",";
          }
        }
        file << "\n";
      }

      file.close();
    }
  };

  const std::vector<T> &getEntries() const { return entries; }

  void addEntry(const T &entry) {
    entries.push_back(entry);
    modified = true;
  }

  void removeEntry(std::function<bool(const T &)> predicate) {
    auto it = std::remove_if(entries.begin(), entries.end(), predicate);
    if (it != entries.end()) {
      entries.erase(it, entries.end());
      modified = true;
    }
  }
};
