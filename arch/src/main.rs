#![no_main]
#![no_std]
#![feature(vec_into_raw_parts)]

use uefi::CStr16;
use uefi::prelude::*;
use uefi::proto::media::fs::SimpleFileSystem;
use uefi::table::boot::{AllocateType, MemoryType, MemoryDescriptor, ScopedProtocol, OpenProtocolParams, OpenProtocolAttributes};
use uefi::proto::console::gop::*;
use uefi::fs::FileSystem;
use goblin::elf::{Elf, program_header};
use core::{mem, ptr, slice};
use uefi_services::println;

extern crate alloc;

#[repr(C)]
#[derive(PartialEq, Eq, PartialOrd, Ord, Debug, Clone, Copy)]
pub struct FrameBufferInfo {
    pub base_addr: u64,
    pub screen_width: u32,
    pub screen_height: u32,
    pub stride: u32
}

#[repr(C)]
#[derive(PartialEq, Eq, PartialOrd, Ord, Debug, Clone, Copy)]
pub struct MemoryMapInfo {
    pub memory_map: *mut MemoryDescriptor,
    pub map_size: u64,
    pub map_desc_size: u64
}

#[repr(C)]
#[derive(PartialEq, Eq, PartialOrd, Ord, Debug, Clone, Copy)]
pub struct BootInfo {
    pub frame_buffer: *const FrameBufferInfo,
    pub memory_map: *const MemoryMapInfo
}

const UEFI_PAGE_SIZE: usize = 0x1000;

#[entry]
fn main(image_handle: Handle, mut system_table: SystemTable<Boot>) -> Status {
    uefi::helpers::init(&mut system_table).unwrap();
    system_table.stdout().reset(false).unwrap();
    let boot_services = system_table.boot_services();

    //File Init
    let simple_file_system = boot_services.get_image_file_system(image_handle).unwrap();
    
    //Load Kernel
    let entry_point_addr = load_kernel(cstr16!("\\kernel.elf"), boot_services, simple_file_system);
    let entry_point: extern "sysv64" fn(&BootInfo) -> ! = unsafe { mem::transmute(entry_point_addr) };

    //Graphic Init
    let frame_buffer_info = get_frame_buffer_info(boot_services);
    println!("FrameBuffer Info:\n - base_addr: {:#x}\n - screen_width: {}\n - screen_height: {}\n - stride: {}",
        frame_buffer_info.base_addr,
        frame_buffer_info.screen_width,
        frame_buffer_info.screen_height,
        frame_buffer_info.stride,
    );

    //Exit Boot Services
    println!("Now Exit the Boot Services");
    let memory_map_size = boot_services.memory_map_size();
    let entry_size = memory_map_size.entry_size;
    let map_size = memory_map_size.map_size;
    println!("DESC Count: {}", map_size / entry_size);
    let (_system_table_runtime , _iter) = system_table.exit_boot_services(MemoryType::LOADER_DATA);

    //Memory Map Init
    let memory_map_descriptors = _iter.get(0).map_or(ptr::null_mut(), |desc| desc as *const _ as *mut MemoryDescriptor);
    let memory_map = MemoryMapInfo {
        memory_map: memory_map_descriptors,
        map_size: map_size as u64,
        map_desc_size: entry_size as u64
    };

    let boot_info = BootInfo {
        frame_buffer: &frame_buffer_info,
        memory_map: &memory_map
    };

    //To Kernel
    entry_point(&boot_info);
}

fn load_kernel(path: &CStr16, boot_services: &BootServices, simple_file_system: ScopedProtocol<SimpleFileSystem>) -> usize {
    let mut file_system = FileSystem::new(simple_file_system);
    let elf_data = file_system.read(path).expect("read failed");
    let elf = Elf::parse(&elf_data).expect("Failed to parse ELF file");

    let mut dest_start = usize::MAX;
    let mut dest_end = 0;
    for ph in elf.program_headers.iter() {
        if ph.p_type != program_header::PT_LOAD {
            continue;
        }
        dest_start = dest_start.min(ph.p_vaddr as usize);
        dest_end = dest_end.max((ph.p_vaddr + ph.p_memsz) as usize);
    }

    boot_services.allocate_pages(
        AllocateType::Address(dest_start as u64),
        MemoryType::LOADER_DATA,
        (dest_end - dest_start + UEFI_PAGE_SIZE - 1) / UEFI_PAGE_SIZE,
    )
    .expect("Failed to allocate pages for kernel");

    for ph in elf.program_headers.iter() {
        if ph.p_type != program_header::PT_LOAD {
            continue;
        }
        let ofs = ph.p_offset as usize;
        let fsize = ph.p_filesz as usize;
        let msize = ph.p_memsz as usize;
        let dest = unsafe { slice::from_raw_parts_mut(ph.p_vaddr as *mut u8, msize) };
        dest[..fsize].copy_from_slice(&elf_data[ofs..ofs + fsize]);
        dest[fsize..].fill(0);
    }
    println!("Kernel Loaded");

    elf.entry as usize
}

fn get_frame_buffer_info(boot_services: &BootServices) -> FrameBufferInfo {
    let gop_handle = boot_services.get_handle_for_protocol::<GraphicsOutput>().unwrap();
    println!("Got GOP Handle");

    let params = OpenProtocolParams {
        handle: gop_handle,
        agent: boot_services.image_handle(),
        controller: None,
    };
    
    unsafe {
        let mut gop = boot_services.open_protocol::<GraphicsOutput>(params, OpenProtocolAttributes::GetProtocol).unwrap();
        println!("Got GOP INFO");
        let mode_info = gop.current_mode_info();

        let mut frame_buffer = gop.frame_buffer();
        let frame_buffer_base = frame_buffer.as_mut_ptr();

        let resolution = mode_info.resolution();
        let stride = mode_info.stride();

        FrameBufferInfo {
            base_addr: frame_buffer_base as u64,
            screen_width: resolution.0 as u32,
            screen_height: resolution.1 as u32,
            stride: stride as u32
        }
    }
}