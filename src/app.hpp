#pragma once

#include <expected>
#include <memory>

class App {
public:
  ~App();
  App(App &&) noexcept;
  App &operator=(App &&) noexcept;

  static std::expected<App, std::string> create(const char *title);
  std::expected<void, std::string> iterate();

private:
  struct Impl;
  explicit App(std::unique_ptr<Impl> p) noexcept;
  std::unique_ptr<Impl> impl;
};
