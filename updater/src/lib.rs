use semver;
use std::convert::TryFrom;
use std::ffi::CString;

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
    UpdatesAvailable(u64, u64, u64), // String = latest verison
    UpToDate,
    Error,
}

#[no_mangle]
pub extern "C" fn updates_available(current_pbc_version: (u64, u64, u64)) -> UpdateCheckingStatus {
    if let Ok(versions) = fetch_parse_and_filter_releases(current_pbc_version) {
        if let Some(latest_version) = versions.first() {
            let major = latest_version.major;
            let minor = latest_version.minor;
            let patch = latest_version.patch;
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
) -> Result<Vec<semver::Version>, MyError> {
    let releases = self_update::backends::github::ReleaseList::configure()
        .repo_owner("obraunsdorf")
        .repo_name("playbook-creator")
        .build()?
        .fetch()?;

    let versions: Vec<semver::Version> = releases
        .iter()
        .map(|release| {
            semver::Version::parse(release.version()).unwrap() // TODO: avoid unwrap!
        })
        .filter(|version| {
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
        let old_version = (0, 10, 0);
        let versions = fetch_parse_and_filter_releases(old_version).unwrap();
        let latest_version = versions.first().unwrap();

        match updates_available(old_version) {
            UpdateCheckingStatus::UpdatesAvailable(..) => {}
            UpdateCheckingStatus::UpToDate => assert!(false),
            UpdateCheckingStatus::Error => assert!(false),
        }

        let major = latest_version.major;
        let minor = latest_version.minor;
        let patch = latest_version.patch;
        match updates_available((major, minor, patch)) {
            UpdateCheckingStatus::UpdatesAvailable(..) => assert!(false),
            UpdateCheckingStatus::UpToDate => {}
            UpdateCheckingStatus::Error => assert!(false),
        }
    }
}
