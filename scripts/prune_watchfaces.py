import re
import os
import hashlib
import shutil

Import("env")

# A few watchfaces have display names that don't match their folder names.
# This small lookup is a permanent one-time fact, not a user-maintained list.
_FOLDER_OVERRIDES = {
    "Chaos_-_Lorenz":  "Chaos_-_Lorenz_Attractor",
    "Multi_face":      "Multi_face_Watchy",
    "Star_Wars":       "Star_Wars_Aurebesh",
    "Watchy_PS":       "Watchy_PowerShell",
    "Watchy7Seg":      "WatchySevenSegment",
    "X_Marks_Spot":    "X_marks_the_spot",
}

def get_active_watchfaces():
    registry_path = os.path.join(env.get("PROJECT_DIR"), "src", "sdk", "WatchfaceRegistry.cpp")
    if not os.path.exists(registry_path):
        print(f"prune_watchfaces: WARNING: Could not find {registry_path}")
        return None

    active = []
    # Match enabled lines starting with X (not _):  X("Name", func)
    pattern = re.compile(r'^\s*X\s*\(\s*"([^"]+)"')

    with open(registry_path, "r") as f:
        for line in f:
            match = pattern.match(line)
            if match:
                name = match.group(1)
                folder = _FOLDER_OVERRIDES.get(name, name)
                active.append(folder)

    return active


active_folders = get_active_watchfaces()

if active_folders is None:
    print("prune_watchfaces: Skipping (registry not found).")
else:
    parts = ["+<*>", "-<watchfaces/*>"]
    for folder in active_folders:
        parts.append(f"+<watchfaces/{folder}/>")
    parts.append("-<os/WatchfaceDrawWatchyGSR.cpp>")
    filter_str = " ".join(parts)

    print(f"prune_watchfaces: Enabling {len(active_folders)} watchfaces: {', '.join(active_folders)}")

    # Cache invalidation: clean stale build artifacts when selection changes
    build_dir  = env.get("PROJECT_BUILD_DIR")
    env_name   = env.get("PIOENV")
    stamp_path = os.path.join(build_dir, env_name, ".watchface_filter_stamp")

    current_hash = hashlib.md5(filter_str.encode()).hexdigest()
    old_hash = ""
    if os.path.exists(stamp_path):
        with open(stamp_path, "r") as f:
            old_hash = f.read().strip()

    if old_hash != current_hash:
        print("prune_watchfaces: Selection changed — cleaning watchface build artifacts.")
        watchface_build = os.path.join(build_dir, env_name, "src", "watchfaces")
        if os.path.isdir(watchface_build):
            shutil.rmtree(watchface_build)
        os.makedirs(os.path.dirname(stamp_path), exist_ok=True)
        with open(stamp_path, "w") as f:
            f.write(current_hash)

    env.Replace(SRC_FILTER=filter_str)
