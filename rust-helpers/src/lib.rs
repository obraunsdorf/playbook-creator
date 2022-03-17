
struct PBCConfigRust {
    a: bool
}

impl PBCConfigRust { 
    fn get_a(&self) -> bool {
        self.a
    }
}

fn new_box() -> Box<PBCConfigRust> {
    Box::new(PBCConfigRust {
        a: true
    })
}

pub fn print_some() {
    println!("something from rust");
}



#[cxx::bridge]
mod ffi {
    extern "Rust" {
        type PBCConfigRust;
        fn new_box() -> Box<PBCConfigRust>;
        fn get_a(self: &PBCConfigRust) -> bool;
    }
}

#[cfg(test)]
mod tests {
    #[test]
    fn it_works() {
        let result = 2 + 2;
        assert_eq!(result, 4);
    }
}