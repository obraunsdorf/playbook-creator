fn main() {
    cxx_build::bridge("src/lib.rs")
        .compiler("clang")
        .compile("cxx-demo");
}