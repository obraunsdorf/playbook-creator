use core::slice;
use semver;
use std::ffi::CString;
use std::os::raw::c_char;

#[derive(Debug, Clone)]
enum MyError {
    SemVerError,
    SelfUpdateError,
}

impl std::convert::From<self_update::errors::Error> for MyError {
    fn from(_: self_update::errors::Error) -> Self {
        MyError::SelfUpdateError
    }
}

impl std::convert::From<semver::SemVerError> for MyError {
    fn from(_: semver::SemVerError) -> Self {
        MyError::SemVerError
    }
}

#[repr(C)]
pub enum UpdateCheckingStatus {
    UpdatesAvailable(u64, u64, u64), //major, minor, patch, release-text
    UpToDate,
    Error,
}

#[repr(C)]
pub struct CBuffer {
    buf: *mut u8,
    len: usize,
}

struct ReleaseWithBody {
    version: semver::Version,
    description: String,
}

#[no_mangle]
pub extern "C" fn updates_available(
    current_pbc_version_major: u64,
    current_pbc_version_minor: u64,
    current_pbc_version_patch: u64,
    description_result_buffer: &CBuffer,
) -> UpdateCheckingStatus {
    let description_result: &mut [u8] = unsafe {
        slice::from_raw_parts_mut(description_result_buffer.buf, description_result_buffer.len)
    };
    if let Ok(versions) = fetch_parse_and_filter_releases((
        current_pbc_version_major,
        current_pbc_version_minor,
        current_pbc_version_patch,
    )) {
        if let Some(latest_release) = versions.first() {
            let major = latest_release.version.major;
            let minor = latest_release.version.minor;
            let patch = latest_release.version.patch;
            // copy bytes from description into C-provided buffer
            let mut description_result_slice =
                &mut description_result[..latest_release.description.len()];
            description_result_slice.clone_from_slice(latest_release.description.as_bytes());
            UpdateCheckingStatus::UpdatesAvailable(major, minor, patch)
        } else {
            UpdateCheckingStatus::UpToDate
        }
    } else {
        UpdateCheckingStatus::Error
    }
}

fn fetch_parse_and_filter_releases(
    current_pbc_version: (u64, u64, u64),
) -> Result<Vec<ReleaseWithBody>, MyError> {
    let releases = self_update::backends::github::ReleaseList::configure()
        .repo_owner("obraunsdorf")
        .repo_name("playbook-creator")
        .build()?
        .fetch()?;

    let versions: Vec<ReleaseWithBody> = releases
        .iter()
        .map(|release| {
            // TODO: avoid unwrap!
            ReleaseWithBody {
                version: semver::Version::parse(release.version()).unwrap(),
                description: release.body.clone(),
            }
        })
        .filter(|ReleaseWithBody { version, .. }| {
            let (major, minor, patch) = current_pbc_version;
            version.gt(&semver::Version::new(major, minor, patch))
        })
        .collect();

    Ok(versions)
}

mod tests {
    use crate::*;

    #[test]
    fn basic_test() {
        let old_version = (0, 13, 0);
        let versions = fetch_parse_and_filter_releases(old_version).unwrap();
        let latest_version = versions.first().unwrap();

        let (major, minor, patch) = old_version;
        let buf = CBuffer {
            buf: [0u8; 500].as_mut_ptr(),
            len: 500,
        };
        match updates_available(major, minor, patch, &buf) {
            UpdateCheckingStatus::UpdatesAvailable(..) => {
                let description: &[u8] = unsafe { slice::from_raw_parts(buf.buf, buf.len) };
                let desc = String::from_utf8_lossy(description);
                println!("description:\n{}", desc)
            }
            UpdateCheckingStatus::UpToDate => assert!(false),
            UpdateCheckingStatus::Error => assert!(false),
        }

        let buf = CBuffer {
            buf: [0u8; 500].as_mut_ptr(),
            len: 500,
        };
        let major = latest_version.version.major;
        let minor = latest_version.version.minor;
        let patch = latest_version.version.patch;
        match updates_available(major, minor, patch, &buf) {
            UpdateCheckingStatus::UpdatesAvailable(..) => assert!(false),
            UpdateCheckingStatus::UpToDate => {}
            UpdateCheckingStatus::Error => assert!(false),
        }
    }
}
