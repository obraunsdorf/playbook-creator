/* bridge/route.rs
   This file is part of Playbook Creator.

    Playbook Creator is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Playbook Creator is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Playbook Creator.  If not, see <http://www.gnu.org/licenses/>.

    Copyright 2025 Oliver Braunsdorf
*/

//! Route-related bridge implementations


use super::controller::with_controller;
use super::types::Route;

// ========== Opaque Type Methods ==========

pub fn route_name(route: &Route) -> &str {
    &route.name
}

pub fn route_code_name(route: &Route) -> &str {
    &route.code_name
}

// ========== Controller Operations ==========

pub fn pbc_get_route_names() -> Vec<String> {
    with_controller(|ctrl| {
        ctrl.playbook()
            .route_names()
            .into_iter()
            .map(|n| n.into_string())
            .collect()
    })
}

pub fn pbc_get_route_names_sorted() -> Vec<String> {
    with_controller(|ctrl| {
        let mut routes: Vec<_> = ctrl.playbook().routes().collect();
        routes.sort_by(|a, b| {
            a.movement
                .endpoint()
                .unwrap()
                .y
                .partial_cmp(&b.movement.endpoint().unwrap().y)
                .unwrap_or(std::cmp::Ordering::Equal)
        });
        routes.into_iter().map(|route| route.name.clone()).collect()
    })
}

pub fn pbc_get_route(name: &str) -> Result<Box<Route>, String> {
    with_controller(|ctrl| {
        let route = ctrl
            .playbook()
            .get_route(&name.into())
            .map_err(|e| format!("Failed to get route: {}", e))?;

        Ok(Box::new(route.clone()))
    })
}

pub fn pbc_has_route(name: &str) -> bool {
    with_controller(|ctrl| ctrl.playbook().get_route(&name.into()).is_ok())
}
