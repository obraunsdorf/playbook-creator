fn main() {
    cxx_build::bridge("src/lib.rs")
        .compiler("clang")
        .flag_if_supported("-std=c++11")
        .static_crt(true)  // link to MSVCRT statically on Windows (only used on windows)
        .compile("cxx-demo");
}