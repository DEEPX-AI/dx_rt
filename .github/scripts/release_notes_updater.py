import os
import re
import sys
from datetime import datetime
from collections import OrderedDict

def get_env_items(var_name):
    """환경 변수에서 아이템 문자열을 가져옵니다."""
    return os.getenv(var_name, "").strip()

def remove_jira_links(text):
    """JIRA 링크 ([ABC-123](url))와 [ABC-123] 태그를 제거합니다."""
    # "[ABC-123](http://...)" 패턴 제거
    text = re.sub(r'\[[A-Za-z0-9_-]+\]\(https?:\/\/[^\)]+\)', '', text)
    # "[ABC-123]" 패턴 제거 (링크가 없는 경우)
    text = re.sub(r'\[[A-Za-z0-9_-]+\]', '', text)
    return text.strip() # 제거 후에는 줄 끝 공백도 제거 (trim)

def parse_markdown_list(lines_raw):
    """
    주어진 문자열(여러 줄)에서 Markdown 리스트 아이템을 파싱하여 리스트의 리스트로 반환합니다.
    들여쓰기된 하위 아이템과 원본 선행 공백을 유지합니다.
    각 아이템은 들여쓰기를 포함한 원본 줄의 리스트로 저장됩니다.
    """
    parsed_items = []
    current_item_lines = []
    
    for line in lines_raw.split('\n'):
        # 빈 줄은 아이템의 구분자 역할을 할 수 있으므로, 완전히 무시하지 않고 구조를 판단
        if line.strip() == "":
            if current_item_lines:
                parsed_items.append(current_item_lines)
                current_item_lines = []
            continue

        if re.match(r'^\s*- ', line): # 새로운 최상위 리스트 아이템 시작
            if current_item_lines:
                parsed_items.append(current_item_lines)
            current_item_lines = [line]
        elif current_item_lines: # 현재 아이템의 하위 줄
            current_item_lines.append(line)

    if current_item_lines: # 마지막 아이템 저장
        parsed_items.append(current_item_lines)
    
    return parsed_items

def format_markdown_list(parsed_items_list):
    """
    파싱된 리스트 아이템(들여쓰기 포함)을 Markdown 형식 문자열로 포맷합니다.
    """
    if not parsed_items_list:
        return ""
    
    formatted_lines = []
    processed_item_contents = set() # 중복 제거를 위한 셋 (JIRA 제거 후 내용으로 비교)

    for item_lines in parsed_items_list:
        if not item_lines:
            continue

        first_line = item_lines[0]
        # 첫 줄에서 선행 공백과 `- `를 분리
        match = re.match(r'^(\s*)- (.*)', first_line)
        if not match: 
            # `- `로 시작하지 않는 최상위 항목은 유효하지 않다고 보고 스킵
            # (또는 첫 줄에 `- `를 강제로 붙여주는 로직 필요)
            print(f"WARNING: Invalid list item format, skipping: '{first_line}'")
            continue
        
        indent_prefix = match.group(1) # 선행 공백
        # 나머지 내용은 JIRA 링크 제거 후 처리
        content_without_jira = remove_jira_links(first_line).lstrip('- ').strip()
        
        # 중복 검사 (JIRA 제거 후 내용으로 비교)
        if content_without_jira in processed_item_contents:
            continue
        processed_item_contents.add(content_without_jira)

        # 첫 줄 재조합 (들여쓰기 + '- ' + JIRA 제거된 내용)
        formatted_lines.append(f"{indent_prefix}- {content_without_jira}")
        
        # 하위 줄은 원본 들여쓰기를 유지한 채 JIRA 링크만 제거
        for i, sub_line in enumerate(item_lines[1:]):
            if sub_line.strip() == "": # 빈 줄은 유지
                formatted_lines.append(sub_line)
            else:
                # 하위 항목의 들여쓰기는 그대로 유지, JIRA만 제거
                original_indent_match = re.match(r'^(\s*).*', sub_line)
                original_indent = original_indent_match.group(1) if original_indent_match else ''
                
                # JIRA 제거 후 trim
                stripped_sub_content = remove_jira_links(sub_line).strip()
                
                # 하위 항목이 '- '로 시작하는 경우도 있을 수 있으므로 처리
                if stripped_sub_content.startswith('- '):
                    stripped_sub_content = stripped_sub_content.lstrip('- ').strip() # 불필요한 '-' 제거

                formatted_lines.append(f"{original_indent}{stripped_sub_content}")

    return "\n".join(formatted_lines)

def parse_release_notes_file(file_path):
    """
    RELEASE_NOTES.md 파일을 파싱하여 버전별 내용을 구조화합니다.
    각 버전의 원본 텍스트도 함께 저장하여 나중에 필요한 부분만 수정할 수 있도록 합니다.
    """
    versions_data = OrderedDict()
    current_version_key = None
    current_section = None
    section_lines_raw = []
    
    # 원본 파일 내용을 라인별로 저장 (인덱스 추적용)
    original_lines = []
    version_line_ranges = {} # 각 버전의 시작/끝 라인 인덱스 저장

    if not os.path.exists(file_path):
        return OrderedDict(), [], {}

    with open(file_path, 'r', encoding='utf-8') as f:
        original_lines = f.readlines()

    header_found = False
    current_version_start_line = None
    
    for line_idx, line in enumerate(original_lines):
        stripped_line = line.strip()
        
        if not header_found and stripped_line == "# RELEASE_NOTES":
            header_found = True
            continue
        if not header_found:
            continue

        if re.match(r'^## v\d+\.\d+\.\d+', stripped_line): # 새 버전 섹션 시작
            # 이전 버전의 라인 범위 저장
            if current_version_key and current_version_start_line is not None:
                version_line_ranges[current_version_key] = {
                    'start': current_version_start_line,
                    'end': line_idx - 1
                }
            
            if current_version_key and current_section: # 이전 버전의 마지막 서브 섹션 내용 저장
                versions_data[current_version_key][current_section] = parse_markdown_list("\n".join(section_lines_raw))
            
            # 새 버전 시작
            match = re.match(r'^(## (v\d+\.\d+\.\d+(?:-\S+)?(?:\\\+\S+)?))(.*)', stripped_line)
            if match:
                current_version_key = match.group(2).strip()
                current_version_start_line = line_idx
                versions_data[current_version_key] = {
                    "header_line": stripped_line,
                    "Changed": [], "Fixed": [], "Added": []
                }
            else:
                current_version_key = None
            section_lines_raw = []
            current_section = None
            
        elif current_version_key and stripped_line.startswith('### '): # 서브 섹션 시작
            if current_section: # 이전 서브 섹션 내용 저장
                versions_data[current_version_key][current_section] = parse_markdown_list("\n".join(section_lines_raw))
            section_lines_raw = []
            
            if "1. Changed" in stripped_line:
                current_section = "Changed"
            elif "2. Fixed" in stripped_line:
                current_section = "Fixed"
            elif "3. Added" in stripped_line:
                current_section = "Added"
            else:
                current_section = None
        elif current_version_key and current_section: # 섹션 내용 추가
            section_lines_raw.append(line.rstrip('\n'))

    # 마지막 버전 처리
    if current_version_key:
        if current_section and section_lines_raw:
            versions_data[current_version_key][current_section] = parse_markdown_list("\n".join(section_lines_raw))
        if current_version_start_line is not None:
            version_line_ranges[current_version_key] = {
                'start': current_version_start_line,
                'end': len(original_lines) - 1
            }

    return versions_data, original_lines, version_line_ranges

def format_single_version_section(version, version_data):
    """단일 버전 섹션을 Markdown 형식으로 포맷합니다."""
    output_lines = []
    
    # 헤더
    header = version_data.get("header_line", f"## {version} / {datetime.now().strftime('%Y-%m-%d')}")
    output_lines.append(header)
    output_lines.append("")

    # 각 섹션
    for section_name in ["Changed", "Fixed", "Added"]:
        output_lines.append(f"### {['1', '2', '3'][['Changed', 'Fixed', 'Added'].index(section_name)]}. {section_name}")
        content = format_markdown_list(version_data[section_name])
        if content:
            output_lines.append(content)
        output_lines.append("")

    return "\n".join(output_lines)

def get_latest_actual_tag():
    """Git describe를 통해 현재 리포지토리의 가장 최신 태그를 가져옵니다."""
    try:
        import subprocess
        latest_actual_tag_cmd = subprocess.run(
            ['git', 'describe', '--tags', '--abbrev=0', '--match', 'v*'],
            capture_output=True, text=True, check=True
        )
        return latest_actual_tag_cmd.stdout.strip()
    except subprocess.CalledProcessError:
        return None

def merge_section_items(existing_items_list, new_items_list):
    """
    기존 아이템 리스트와 새로운 아이템 리스트를 합치고 중복을 제거합니다.
    각 아이템은 원본 줄 리스트 ([줄1, 줄2, ...]) 형태입니다.
    """
    combined_items = existing_items_list[:] # 기존 리스트 복사
    
    # 새로운 아이템들을 추가하기 전, JIRA 제거된 내용으로 중복 검사 셋 생성
    existing_contents = set()
    for item_lines in existing_items_list:
        if item_lines:
            existing_contents.add(remove_jira_links(item_lines[0]).lstrip('- ').strip())

    for new_item_lines in new_items_list:
        if not new_item_lines:
            continue
        # 새로운 아이템의 첫 줄 내용 (JIRA 제거 후)
        new_content_check = remove_jira_links(new_item_lines[0]).lstrip('- ').strip()
        
        # 중복이 아니면 추가
        if new_content_check not in existing_contents:
            combined_items.append(new_item_lines)
            existing_contents.add(new_content_check)

    # 정렬
    combined_items.sort(key=lambda item_lines: remove_jira_links(item_lines[0]).lstrip('- ').strip().lower())
    
    return combined_items

def update_release_notes_selectively(file_path, versions_data, original_lines, version_line_ranges, 
                                   target_version, new_version_data, operation_type, versions_to_remove=None):
    """
    RELEASE_NOTES.md 파일을 선택적으로 업데이트합니다.
    operation_type: 'update_existing', 'add_new', 'merge_and_add'
    """
    new_lines = original_lines[:]
    
    if operation_type == 'update_existing':
        # 기존 버전 섹션을 업데이트
        if target_version in version_line_ranges:
            range_info = version_line_ranges[target_version]
            start_line = range_info['start']
            end_line = range_info['end']
            
            # 새로운 섹션 내용 생성
            new_section_content = format_single_version_section(target_version, new_version_data)
            new_section_lines = new_section_content.split('\n')
            
            # 기존 라인들을 새로운 내용으로 교체
            new_lines[start_line:end_line + 1] = [line + '\n' for line in new_section_lines]
            
    elif operation_type == 'add_new':
        # 새로운 버전 섹션을 맨 위에 추가
        new_section_content = format_single_version_section(target_version, new_version_data)
        new_section_lines = new_section_content.split('\n')
        
        # 헤더 다음에 삽입 (# RELEASE_NOTES 다음)
        header_index = 0
        for i, line in enumerate(new_lines):
            if line.strip() == "# RELEASE_NOTES":
                header_index = i + 1
                break
        
        # 빈 줄이 있다면 그 다음에, 없다면 바로 다음에 삽입
        insert_index = header_index + 1 if header_index + 1 < len(new_lines) and new_lines[header_index].strip() == "" else header_index
        
        # 새로운 섹션 삽입
        for line in reversed(new_section_lines):
            new_lines.insert(insert_index, line + '\n')
            
    elif operation_type == 'merge_and_add':
        # 기존 버전들을 제거하고 새 버전 추가
        if versions_to_remove:
            # 뒤에서부터 제거 (인덱스 변화 방지)
            ranges_to_remove = []
            for version in versions_to_remove:
                if version in version_line_ranges:
                    ranges_to_remove.append(version_line_ranges[version])
            
            # 라인 범위를 끝 라인 기준으로 역순 정렬하여 뒤에서부터 제거
            ranges_to_remove.sort(key=lambda x: x['end'], reverse=True)
            
            for range_info in ranges_to_remove:
                start_line = range_info['start']
                end_line = range_info['end']
                # 버전 섹션과 그 다음 빈 줄까지 제거
                del new_lines[start_line:end_line + 2 if end_line + 1 < len(new_lines) and new_lines[end_line + 1].strip() == "" else end_line + 1]
        
        # 새로운 버전 섹션을 맨 위에 추가
        new_section_content = format_single_version_section(target_version, new_version_data)
        new_section_lines = new_section_content.split('\n')
        
        # 헤더 다음에 삽입
        header_index = 0
        for i, line in enumerate(new_lines):
            if line.strip() == "# RELEASE_NOTES":
                header_index = i + 1
                break
        
        insert_index = header_index + 1 if header_index + 1 < len(new_lines) and new_lines[header_index].strip() == "" else header_index
        
        for line in reversed(new_section_lines):
            new_lines.insert(insert_index, line + '\n')
    
    # 파일에 쓰기
    with open(file_path, 'w', encoding='utf-8') as f:
        f.writelines(new_lines)

def main():
    if len(sys.argv) != 4:
        print("Usage: python release_notes_updater.py <new_version_tag> <base_version_tag> <bump_type>")
        sys.exit(1)
    
    new_version_tag = sys.argv[1] 
    base_version_tag = sys.argv[2]
    bump_type = sys.argv[3]       

    print(f"--- Release Notes Automation Start --- (Current Time: {datetime.now()})")
    print(f"New Version Tag (calculated by semver-action): {new_version_tag}")
    print(f"Base Version Tag (input): {base_version_tag}")
    print(f"Bump Type: {bump_type}")

    # 릴리즈 노트 재료 가져오기
    changed_items_material = get_env_items("CHANGED_ITEMS")
    fixed_items_material = get_env_items("FIXED_ITEMS")
    added_items_material = get_env_items("ADDED_ITEMS")

    print(f"Collected Items - Changed: {len(changed_items_material)} chars, Fixed: {len(fixed_items_material)} chars, Added: {len(added_items_material)} chars")

    release_notes_path = "RELEASE_NOTES.md"
    existing_versions_data, original_lines, version_line_ranges = parse_release_notes_file(release_notes_path)
    
    if not existing_versions_data:
        print("Existing RELEASE_NOTES.md is empty or not found. Starting with a blank slate for new content.")
    else:
        print(f"Existing RELEASE_NOTES.md parsed. Found {len(existing_versions_data)} versions.")

    # 현재 리포지토리의 가장 최신 태그 가져오기
    latest_actual_tag = get_latest_actual_tag()
    print(f"Latest actual tag in repo (from git describe): {latest_actual_tag}")
    
    # 새로운 릴리즈 노트 재료 (parsed list of items)
    new_material_parsed = {
        "Changed": parse_markdown_list(changed_items_material),
        "Fixed": parse_markdown_list(fixed_items_material),
        "Added": parse_markdown_list(added_items_material)
    }

    # 릴리즈 노트에 새로운 내용이 있는지 확인
    has_new_material = any(new_material_parsed[sec] for sec in new_material_parsed)

    # 새로운 섹션 데이터 구조
    new_section_data = {
        "header_line": f"## {new_version_tag} / {datetime.now().strftime('%Y-%m-%d')}",
        "Changed": new_material_parsed["Changed"],
        "Fixed": new_material_parsed["Fixed"],
        "Added": new_material_parsed["Added"]
    }

    # 시나리오별 처리
    if bump_type == "none" and new_version_tag == base_version_tag and has_new_material:
        print(f"Scenario 1: No version bump ({new_version_tag}). Appending to this version's section.")
        if new_version_tag in existing_versions_data:
            # 기존 섹션에 새 내용 병합
            updated_data = existing_versions_data[new_version_tag].copy()
            updated_data["Changed"] = merge_section_items(updated_data["Changed"], new_section_data["Changed"])
            updated_data["Fixed"] = merge_section_items(updated_data["Fixed"], new_section_data["Fixed"])
            updated_data["Added"] = merge_section_items(updated_data["Added"], new_section_data["Added"])
            
            update_release_notes_selectively(release_notes_path, existing_versions_data, original_lines, 
                                           version_line_ranges, new_version_tag, updated_data, 'update_existing')
        else:
            # 새로운 섹션 추가
            update_release_notes_selectively(release_notes_path, existing_versions_data, original_lines, 
                                           version_line_ranges, new_version_tag, new_section_data, 'add_new')

    elif bump_type != "none" and base_version_tag == latest_actual_tag and has_new_material:
        print(f"Scenario 2: Version bump from latest ({base_version_tag} -> {new_version_tag}). Creating new section at top.")
        update_release_notes_selectively(release_notes_path, existing_versions_data, original_lines, 
                                       version_line_ranges, new_version_tag, new_section_data, 'add_new')
        
    elif bump_type != "none" and new_version_tag == latest_actual_tag and new_version_tag != base_version_tag and has_new_material:
        print(f"Scenario 3: Re-bump for existing version ({new_version_tag}) from an older base ({base_version_tag}). Appending to existing section.")
        if new_version_tag in existing_versions_data:
            updated_data = existing_versions_data[new_version_tag].copy()
            updated_data["Changed"] = merge_section_items(updated_data["Changed"], new_section_data["Changed"])
            updated_data["Fixed"] = merge_section_items(updated_data["Fixed"], new_section_data["Fixed"])
            updated_data["Added"] = merge_section_items(updated_data["Added"], new_section_data["Added"])
            
            update_release_notes_selectively(release_notes_path, existing_versions_data, original_lines, 
                                           version_line_ranges, new_version_tag, updated_data, 'update_existing')
        else:
            print(f"ERROR: Scenario 3: Calculated version '{new_version_tag}' matches latest actual tag, but not found in RELEASE_NOTES.md. Aborting.")
            sys.exit(1)

    elif bump_type != "none" and new_version_tag != latest_actual_tag and (bump_type == "major" or bump_type == "minor") and has_new_material:
        print(f"Scenario 4: Major/Minor bump from an older base ({base_version_tag} -> {new_version_tag}). Merging previous section(s) into new section.")
        
        # Version 클래스가 없으므로 간단한 비교 함수 사용
        def version_compare(v1, v2):
            """간단한 버전 비교 함수"""
            def parse_version(v):
                return list(map(int, v.lstrip('v').split('.')))
            return parse_version(v1), parse_version(v2)
        
        base_v_parts, _ = version_compare(base_version_tag, new_version_tag)
        new_v_parts, _ = version_compare(new_version_tag, base_version_tag)
        
        # 병합할 버전들과 제거할 버전들 찾기
        versions_to_merge_content = {"Changed": [], "Fixed": [], "Added": []}
        versions_to_remove = []
        
        for ver_tag, ver_data in existing_versions_data.items():
            try:
                current_v_parts, _ = version_compare(ver_tag, base_version_tag)
                base_parts, _ = version_compare(base_version_tag, ver_tag)
                new_parts, _ = version_compare(new_version_tag, ver_tag)
                
                # base_version_tag < current_version < new_version_tag 범위의 버전들
                if (current_v_parts > base_parts and current_v_parts < new_parts):
                    print(f"DEBUG: Preparing to merge content from old section {ver_tag} into {new_version_tag}")
                    versions_to_merge_content["Changed"].extend(ver_data["Changed"])
                    versions_to_merge_content["Fixed"].extend(ver_data["Fixed"])
                    versions_to_merge_content["Added"].extend(ver_data["Added"])
                    versions_to_remove.append(ver_tag)
            except Exception as e:
                print(f"WARNING: Could not parse version tag '{ver_tag}' during merge: {e}. Skipping.")
                continue
        
        # 새로운 섹션에 기존 내용들 병합
        merged_section_data = new_section_data.copy()
        merged_section_data["Changed"] = merge_section_items(merged_section_data["Changed"], versions_to_merge_content["Changed"])
        merged_section_data["Fixed"] = merge_section_items(merged_section_data["Fixed"], versions_to_merge_content["Fixed"])
        merged_section_data["Added"] = merge_section_items(merged_section_data["Added"], versions_to_merge_content["Added"])
        
        # 병합된 버전들 제거하고 새 버전 추가
        update_release_notes_selectively(release_notes_path, existing_versions_data, original_lines, 
                                       version_line_ranges, new_version_tag, merged_section_data, 
                                       'merge_and_add', versions_to_remove)

    else:
        if not has_new_material and bump_type == "none":
            print("No version bump and no new material. Skipping RELEASE_NOTES.md update.")
            sys.exit(0)
        else:
            print(f"WARNING: Unhandled scenario or unexpected input combination.")
            print(f"New Version: {new_version_tag}, Base Version: {base_version_tag}, Bump Type: {bump_type}, Latest Actual Tag: {latest_actual_tag}")
            sys.exit(1)
            
    print(f"✅ RELEASE_NOTES.md updated successfully for version {new_version_tag}!")
    print(f"--- Release Notes Automation Completed ---")

if __name__ == "__main__":
    main()
