fn main() {
    cxx_build::bridge("src/lib.rs")
        .compiler("clang")
        .flag_if_supported("-std=c++11")
        .compile("cxx-demo");
}